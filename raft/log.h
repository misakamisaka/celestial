#ifndef _CELESTIAL_LOG_H_
#define _CELESTIAL_LOG_H_

namespace celestial {
class Log {
public:
    // init log
    virtual void init(Configure& conf) = 0;
    // append entries to log
    virtual void append(const std::vector<shared_ptr<Entry> >& entries);
    // truncate log between [0, index)
    virtual void truncateIndexBefore(int64_t index) = 0;
    // truncate log between (index, last_index]
    virtual void truncateIndexAfter(int64_t index) = 0;

    virtual shared_ptr<Entry> getEntryFromIndex(int64_t index) = 0;

    virtual int64_t getStartIndex() = 0;

    virtual int64_t getLastIndex() = 0;
    
    virtual bool isNeedSnapshot() = 0;
};
}

#endif
