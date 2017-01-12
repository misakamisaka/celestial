#include "election_timeout_checker.h"

namespace celestial {
ElectionTimeoutChecker::RaftElectionTimeoutChecker()
    : ELECTION_TIMEOUT(Configure["election_timeout"].asint64()),
    generator_(std::chrono::steady_clock::now()),
    distribution_(ELECTION_TIMEOUT, ELECTION_TIMEOUT*2),
    exiting_(false) {
}

void ElectionTimeoutChecker::run() {
    using namespace std::chrono;
    std::unique_lock<std::mutex> lock(mutex_);
    while(!exiting_) {
        if (steady_clock::now() >= election_start_time_point_) {
            raft_manager_->startNewElection();
        }
        time_changed_.wait_until(lock, election_start_time_point_);
    }
}

void ElectionTimeoutChecker::reset() {
    using namespace std::chrono;
    nanoseconds duration(distribution_(generator_));
    std::unique_lock<std::mutex> lock(mutex_);
    election_start_time_point_ = steady_clock::now() + duration;
    time_changed_.notify_one();
}

void ElectionTimeoutChecker::setMax() {
    using namespace std::chrono;
    std::unique_lock<std::mutex> lock(mutex_);
    election_start_time_point_ = steady_clock::time_point::max();
    time_changed_.notify_one();
}
}
