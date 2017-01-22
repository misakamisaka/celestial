#include "log.h"

namespace celestial{
void Log::init(Configure& conf) {
}
void Log::append(const std::vector<shared_ptr<Entry> >& entries) {
    //check if entries valid
    //lock 
    //while {
    //get next index
    //append to log segment
    //check if need roll
    //update next position
    //}
}
void Log::truncateIndexBefore(int64_t index) {
    //lock
    //find log segment
    //delete segments before the segment
}
void Log::truncateIndexAfter(int64_t index) {
    //lock
    //find log segment
    //delete segments after the segment
    //truncate the log segment after index
}

shared_ptr<Entry> Log::getEntryFromIndex(int64_t index) {
    //lock
    //get the coresponding segment
    //find the segment by index
    //read from segment
}

int64_t Log::getStartIndex() {
}

int64_t Log::getLastIndex(){ 
}

void Log::roll() {
}
}
