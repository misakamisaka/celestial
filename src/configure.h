#ifndef _CELESTIAL_CONFIGURE_H_
#define _CELESTIAL_CONFIGURE_H_

#include <string>

namespace celestial {
class Configure {
public:
    static Configure& instance() {
        static Configure configure;
        return configure;
    }
    void init(std::string conf_str);
    std::string get_log_dir() {
        return log_dir_;
    }
    std::string get_log_prefix() {
        return log_prefix_;
    }
    uint32_t get_log_roll_size() {
        return log_roll_size_;
    }
    std::string get_snapshot_dir() {
        return snapshot_dir_;
    }
    std::string get_snapshot_name() {
        return snapshot_name_;
    }
    uint32_t get_snapshot_max_block_size() {
        return snapshot_max_block_size_;
    }
private:
    Configure() = default;
    Configure(const Configure&) = delete;
    Configure& operator=(const Configure&) = delete;
private:
    std::string log_dir_;
    std::string log_prefix_;
    uint32_t log_roll_size_;
    std::string snapshot_dir_;
    std::string snapshot_name_;
    uint32_t snapshot_max_block_size_;
};
}

#endif
