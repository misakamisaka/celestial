#include "log.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include "raft_service_types.h"
#include "log_file.h"
#include "log_exception.h"
#include "configure.h"

using std::string;
using std::map;
using std::vector;
using std::shared_ptr;

namespace celestial{
Log::Log() {
    log_dir_ = Configure::instance().get_log_dir();
    log_prefix_ = Configure::instance().get_log_prefix();
    log_roll_size_ = Configure::instance().get_log_roll_size();
    namespace bf = boost::filesystem;
    bf::path filepath(log_dir_);
    bf::directory_iterator end_it;
    for (bf::directory_iterator file_it(filepath); file_it != end_it; ++file_it) {
        string fname = file_it->path().filename().string();
        if (boost::algorithm::starts_with(fname, log_prefix_)) {
            size_t last_point_pos = fname.find_last_of('.');
            try {
                int start_index = boost::lexical_cast<int64_t>(fname.substr(last_point_pos + 1));
                index_file_map_.insert(std::make_pair(start_index, std::make_shared<LogFile>(fname)));
            } catch (boost::bad_lexical_cast &e) {
                LOG(INFO) << "filename[" + fname + "] has no index";
            }
        }
    }
    if (index_file_map_.empty()) {
        string filename = log_dir_ + "/" + log_prefix_ + "1";
        index_file_map_.insert(std::make_pair(1, std::make_shared<LogFile>(filename)));
    }
    last_file_ = index_file_map_.rbegin()->second;
    start_index_ = index_file_map_.begin()->second->getStartIndex();
    last_index_ = index_file_map_.rbegin()->second->getLastIndex();
}
void Log::append(const std::vector<shared_ptr<Entry> >& entries) {
    //check if entries valid
    //lock 
    std::lock_guard<std::mutex> lock(mutex_);
    //check roll
    if (last_file_->getFileSize() >= log_roll_size_) {
        roll();
    }
    //append to last_files;
    last_file_->append(entries);
}
void Log::truncateToIndex(int64_t index) {
    if (index < start_index_ || index > last_index_) {
        throw LogException("index[" + std::to_string(index) + "] is not between start index["
                + std::to_string(start_index_) + 
                "] and last index[" + std::to_string(last_index_) + "]");
    }
    //lock
    std::lock_guard<std::mutex> lock(mutex_);
    //delete segments before the segment
    auto seg_it = index_file_map_.upper_bound(index);
    --seg_it;
    start_index_ = index;

    for (auto it = index_file_map_.begin(); it != seg_it;) {
        it->second->setDelete();
        it = index_file_map_.erase(it);
    }
}
void Log::truncateFromIndex(int64_t index) {
    if (index < start_index_ || index > last_index_) {
        throw LogException("index[" + std::to_string(index) + "] is not between start index["
                + std::to_string(start_index_) + 
                "] and last index[" + std::to_string(last_index_) + "]");
    }
    //lock
    std::lock_guard<std::mutex> lock(mutex_);
    //truncate the log segment after index
    auto seg_it = index_file_map_.upper_bound(index);
    --seg_it;
    seg_it->second->truncateFromIndex(index);
    
    last_file_ = seg_it->second;
    last_index_ = index - 1;
    //delete segments after the segment
    ++seg_it;
    for (auto it = seg_it; it != index_file_map_.end();) {
        it->second->setDelete();
        it = index_file_map_.erase(it);
    }
}

void Log::getEntryFromIndex(int64_t index, uint32_t& number_of_entry, 
    std::vector<std::shared_ptr<Entry> >& entries_result) {
    //lock
    std::lock_guard<std::mutex> lock(mutex_);
    //find the segment by index
    auto seg_it = index_file_map_.upper_bound(index);
    --seg_it;
    //read from segment
    seg_it->second->getEntryFromIndex(index, number_of_entry, entries_result);
}

int64_t Log::getStartIndex() {
    return start_index_;
}

int64_t Log::getLastIndex(){ 
    return last_index_;
}

void Log::roll() {
    int64_t next_index = index_file_map_.rbegin()->second->getLastIndex() + 1;
    string filename = log_dir_ + "/" + log_prefix_ + std::to_string(next_index);
    index_file_map_.insert(std::make_pair(next_index, std::make_shared<LogFile>(filename)));
    last_file_ = index_file_map_.rbegin()->second;
}
}
