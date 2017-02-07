#ifndef _CELESTIAL_SNAPSHOT_H_
#define _CELESTIAL_SNAPSHOT_H_

#include <memory>
#include <string>
#include <fstream>

namespace celestial {
class Snapshot {
public:
    Snapshot();

    void serializeSnapshot(std::shared_ptr<StateMachine> state_machine);
    std::shared_ptr<StateMachine> deserializeSnapshot();

    //raft peer call the following method
    int64_t getLastIncludedIndex();
    int64_t getLastIncludedTerm();
    std::string readDataFromOffset(int64_t offset);
    uint32_t getFileSize();
    
    //raft service call the following method
    void setLastIncludedIndex(int64_t last_index);
    void setLastIncludedTerm(int64_t last_term);
    void writeDataFromOffset(const std::string& data, int64_t offset);

private:
    int64_t last_index_;
    int64_t last_term_;
    std::string filepath_;
    uint32_t snapshot_max_block_size_;
};
}

#endif
