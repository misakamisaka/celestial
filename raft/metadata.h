#ifndef _CELESTIAL_METADATA_H_
#define _CELESTIAL_METADATA_H_

#include <string>

namespace celestial {
class Metadata {
public:
    void init();
    void save();
    void load();

    std::string path;
    int32_t index;
    int64_t current_term;
    int64_t voted_for;
};
}

#endif
