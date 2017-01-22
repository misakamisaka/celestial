#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <memory>
#include <fstream>
#include <string>
#include <vector>

namespace celestial {

class Entry;

struct LogEntryHeader {
    uint32_t entry_size;
    void fromStream(std::istream &is) {
        is.read((char*)&entry_size, sizeof(entry_size));
    }
    void toStream(std::ostream &os) {
        os.write((char*)&entry_size, sizeof(entry_size));
    }
};

class LogFile {
public:
    LogFile(const std::string& filepath);
    ~LogFile();
    void append(const std::vector<std::shared_ptr<Entry> >& entries);
    void getEntryFromIndex(int64_t index, uint32_t& number_of_entry, 
        std::vector<std::shared_ptr<Entry> >& entries_result);
    void truncateFromIndex(int64_t index);
private:
    std::string filepath_;
    std::fstream fs_;
    int64_t start_index_;
    uint32_t number_of_entry_;
    std::vector<uint32_t> position_vec_;
};
}

#endif
