#ifndef _CELESTIAL_LOG_H_
#define _CELESTIAL_LOG_H_

namespace celestial {

class Log {
public:
    // init log
    void init(Configure& conf);
    // append entries to log
    void append(const std::vector<shared_ptr<Entry> >& entries);
    // truncate log between [0, index)
    void truncateIndexBefore(int64_t index);
    // truncate log between (index, last_index]
    void truncateIndexAfter(int64_t index);

    shared_ptr<Entry> getEntryFromIndex(int64_t index) ;

    int64_t getStartIndex();

    int64_t getLastIndex();

    void roll();
private:
    std::string dir;
    std::map<int64_t, std::shared_ptr<LogSegment> > index_segment_map_;
};
}

#endif
