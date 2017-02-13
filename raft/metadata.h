#ifndef _CELESTIAL_METADATA_H_
#define _CELESTIAL_METADATA_H_

#include <string>

namespace celestial {
class MetaData {
public:
	MetaData(){
		current_term = 0;
		voted_for = 0;
	}
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
