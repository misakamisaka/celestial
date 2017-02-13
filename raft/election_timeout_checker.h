#ifndef _CELESTIAL_RAFT_ELECTION_TIMEOUT_CHECKER_H_
#define _CELESTIAL_RAFT_ELECTION_TIMEOUT_CHECKER_H_

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <random>
#include "runnable.h"
#include <iostream>
#include <ctime>

namespace celestial {
class ElectionTimeoutChecker : public Runnable {
public:
    ElectionTimeoutChecker();
    void init();
    void run();
	
    void reset();
    void setMax();
    void exit() {
        exiting_ = true;
    }
private:
    const int64_t ELECTION_TIMEOUT;
    //std::chrono::steady_clock<std::chrono::time_point> election_start_time_point_;
	std::chrono::steady_clock::time_point election_start_time_point_;
    mutable std::mutex mutex_;
    std::condition_variable time_changed_;
    std::default_random_engine generator_;
    std::uniform_int_distribution<int64_t> distribution_;
    bool exiting_;

    //std::shared_ptr<RaftContext> raft_context_;
    //std::shared_ptr<RaftManager> raft_manager_;

};
}

#endif
