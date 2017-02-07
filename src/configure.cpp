#include "configure.h"
#include <json/json.h>
#include <glog/logging.h>
#include "celestial_exception.h"

using std::string;

namespace celestial {
void Configure::init(std::string conf_str) {
    Json::Value config;
    Json::Reader reader;
    reader.parse(conf_str, config);

    try {
        log_dir_ = config["log_dir"].asString();
        log_prefix_ = config["log_prefix"].asString();
        log_roll_size_ = config["log_roll_size"].asUInt();
        snapshot_dir_ = config["snapshot_dir"].asString();
        snapshot_name_ = config["snapshot_name"].asString();
        snapshot_max_block_size_ = config["snapshot_max_block_size"].asUInt();
    } catch (std::exception &e) {
        throw CelestialException(string("log init failed, error:") + e.what());
    }
}
}
