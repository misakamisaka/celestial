#ifndef _CELESTIAL_SNAPSHOT_H_
#define _CELESTIAL_SNAPSHOT_H_

namespace celestial {
class Snapshot {
public:
    virtual void serializeSnapshot() = 0;
    virtual void deserializeSnapshot() = 0;
    int64_t getLastIncludedIndex() = 0;
    int64_t getLastIncludedTerm() = 0;
    void setLastIncludedIndex(int64_t last_index) = 0;
    void setLastIncludedTerm(int64_t last_term) = 0;
    std::string readDataFromOffset(int64_t offset) = 0;
    void writeDataFromOffset(const std::String& data, int64_t offset) = 0;
    int64_t getFileSize() = 0;
private:
    std::shared_ptr<StateMachine> state_machine_;
};
}

#endif
