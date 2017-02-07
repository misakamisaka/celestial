#include "snapshot.h"
#include <boost/filesystem.hpp>

namespace celestial {
Snapshot::Snapshot() {
    filepath_ = Configure::instance().get_snapshot_dir() + "/" + Configure::instance().get_snapshot_name();
    snapshot_max_block_size_ = Configure::instance().get_snapshot_max_block_size();
}

void Snapshot::serializeSnapshot(std::shared_ptr<StateMachine> state_machine) {
    std::string filepath = filepath_ + "_" + std::tostring(last_index_) + "_" + std::tostring(last_term_);
    std::string tmp_filepath = filepath + ".tmp"
    std::fstream fs(tmp_filepath, std::fstream::out | std::fstream::binary | std::fstream::trunc);
    state_machine->writeToStream(fs);
    fs.close();
    boost::filesystem::rename(tmp_filepath, filepath);
}
std::shared_ptr<StateMachine> Snapshot::deserializeSnapshot() {
    std::string filepath = filepath_ + "_" + std::tostring(last_index_) + "_" + std::tostring(last_term_);
    std::fstream fs(filepath, std::fstream::in | std::fstream::binary);
    std::shared_ptr<StateMachine> state_machine = make_shared<StateMachine>();
    state_machine->readFromStream(fs);
    fs.close();
    return state_machine;
}
int64_t Snapshot::getLastIncludedIndex() {
    return last_index_;
}
void Snapshot::setLastIncludedIndex(int64_t last_index) {
    last_index_ = last_index;
}
int64_t Snapshot::getLastIncludedTerm() {
    return last_term_;
}
void Snapshot::setLastIncludedTerm(int64_t last_term) {
    last_term_ = last_term;
}
std::string Snapshot::readDataFromOffset(int64_t offset) {
    std::string filepath = filepath_ + "_" + std::tostring(last_index_) + "_" + std::tostring(last_term_);
    std::fstream fs(filepath, std::fstream::in | std::fstream::binary);
    fs.seekg(offset, std::ios::beg);
    char* tmp = new char[snapshot_max_block_size_];
    fs.read(tmp, snapshot_max_block_size_);
    uint32_t size = fs.gcount();
    string res(tmp, size);
    delete tmp;
    return res;
}
void Snapshot::writeDataFromOffset(const std::string& data, int64_t offset) {
    std::string filepath = filepath_ + "_" + std::tostring(last_index_) + "_" + std::tostring(last_term_);
    if (boost::filesystem::exists(filepath)) {
        try {
            boost::filesystem::resize_file(filepath, offset);
        } catch (std::exception &e) {
            throw SnapshotException(string("truncate fail, error:") + e.what());
        }
    }
    std::fstream fs(filepath, std::fstream::out | std::fstream::binary);
    fs.seekp(offset, std::ios::beg);
    fs.write(data.c_str(), data.size());
}
uint32_t Snapshot::getFileSize() {
    return boost::filesystem::file_size(filepath_);
}
}
