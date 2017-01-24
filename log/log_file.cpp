#include "log_file.h"
#include <boost/shared_ptr.hpp>
#include <glog/logging.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "log_exception.h"
#include "raft_service_types.h"

using std::string;
using std::vector;
using apache::thrift::protocol::TBinaryProtocol; 
using apache::thrift::transport::TMemoryBuffer;

namespace celestial {
LogFile::LogFile(const std::string& filepath)
    : filepath_(filepath), need_delete_(false) {
    filepath_ = filepath;
    size_t last_point_pos = filepath_.find_last_of('.');
    start_index_ = boost::lexical_cast<int64_t>(filepath_.substr(last_point_pos+1));
    fs_.open(filepath_, std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::app);
    fs_.seekg(0,  std::ios::beg);
    position_vec_.push_back(0);
    while (true) {
        LogEntryHeader header;
        header.fromStream(fs_);
        if (fs_.eof()) {
            fs_.clear();
            break;
        }
        fs_.seekg(header.entry_size, std::ios::cur);
        position_vec_.push_back(fs_.tellg());
    } 
}
LogFile::~LogFile() {
    if (fs_.is_open()) {
        fs_.close();
    }
    if (need_delete_) {
        boost::filesystem::remove(filepath_);
    }
}
void LogFile::append(const std::vector<std::shared_ptr<Entry> >& entries) {
    fs_.seekp(0, std::ios::end);
    for (auto &entry : entries) {
        boost::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
        boost::shared_ptr<TBinaryProtocol> binary_protcol(new TBinaryProtocol(buffer));
        entry->write(binary_protcol.get());
        string entry_raw = buffer->getBufferAsString();
        LogEntryHeader header({(uint32_t)entry_raw.size()});
        header.toStream(fs_);
        fs_.write(entry_raw.data(), entry_raw.size());
        position_vec_.push_back(fs_.tellp());
    }
}
void LogFile::getEntryFromIndex(int64_t index, uint32_t& number_of_entry, 
        std::vector<std::shared_ptr<Entry> >& entries_result) {
    if (index >= start_index_ + position_vec_.size() - 1 || index < start_index_) {
        throw LogException("cannot find index[" + std::to_string(index) + "]");
    }
    fs_.seekg(position_vec_[index - start_index_], std::ios::beg);
    uint32_t i = 0;
    for (; i < number_of_entry; ++i) {
        LogEntryHeader header;
        header.fromStream(fs_);
        if (fs_.eof()) {
            fs_.clear();
            break;
        }
        char entry_data[header.entry_size];
        fs_.read(entry_data, header.entry_size);
        if (!fs_.good()) {
            throw LogException("position is not current or entry is broken");
        }
        std::shared_ptr<Entry> entry = std::make_shared<Entry>();
        try {
            boost::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
            boost::shared_ptr<TBinaryProtocol> binary_protcol(new TBinaryProtocol(buffer));
            buffer->resetBuffer((uint8_t*)entry_data, header.entry_size);
            entry->read(binary_protcol.get());
        } catch (apache::thrift::TException&e ){
            throw LogException("broken entry!");
        }
        entries_result.push_back(entry);
    }
    number_of_entry = i;
}
void LogFile::truncateFromIndex(int64_t index) {
    if (index >= start_index_ + position_vec_.size() - 1 || index < start_index_) {
        throw LogException("cannot find index[" + std::to_string(index) + "]");
    }
    position_vec_.erase(position_vec_.begin() + (index - start_index_) + 1, position_vec_.end()); 
    if (fs_.is_open()) {
        fs_.close();
    }
    if (truncate(filepath_.c_str(), position_vec_[index - start_index_]) != 0) {
        throw LogException("truncate fail, error:" + errno);
    }
    fs_.open(filepath_, std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::app);
    fs_.seekg(0, std::ios::end);
}
}
