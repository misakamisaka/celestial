#ifndef OFFSET_INDEX_H
#define OFFSET_INDEX_H

namespace celestial {
class OffsetIndex {
public:
    void append();
    int64_t getPositionByOffset();
};
}

#endif
