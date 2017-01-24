#ifndef _CELESTIAL_LOG_H_
#define _CELESTIAL_LOG_H_

#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include <json/json.h>

namespace celestial {

class Entry;
class LogFile;

class Log {
public:
    // init log
    void init(Json::Value& config);
    // append entries to log
    void append(const std::vector<std::shared_ptr<Entry> >& entries);
    // truncate log between [0, index)
    void truncateToIndex(int64_t index);
    // truncate log between (index, last_index]
    void truncateFromIndex(int64_t index);

    void getEntryFromIndex(int64_t index, uint32_t& number_of_entry, 
        std::vector<std::shared_ptr<Entry> >& entries_result);

    int64_t getStartIndex();

    int64_t getLastIndex();

    void roll();
private:
    std::mutex mutex_;
    std::string dir_;
    std::string log_prefix_;
    std::map<int64_t, std::shared_ptr<LogFile> > index_file_map_;
    std::shared_ptr<LogFile> last_file_;
    uint32_t log_roll_size_;
    int64_t start_index_;
    int64_t last_index_;
};
}

#endif
