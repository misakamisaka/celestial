#ifndef LOG_OFFSET_METADATA_H
#define LOG_OFFSET_METADATA_H

namespace celestial {
class LogOffsetMetadata {
public:
    bool onOlderSegment(const LogOffsetMetadata& that);
    bool onSameSegment(const LogOffsetMetadata& that);
    int64_t offsetDiff(const LogOffsetMetadata& that);
    int64_t positionDiff(const LogOffsetMetadata& that);
    bool messageOnly(); 
private:
    int64_t message_offset_;
    int64_t segment_base_offset_;
    int64_t relative_position_in_segment_;
};
}

#endif
