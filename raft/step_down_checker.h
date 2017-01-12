#ifndef _CELESTIAL_RAFT_ELECTION_TIMEOUT_CHECKER_H_
#define _CELESTIAL_RAFT_ELECTION_TIMEOUT_CHECKER_H_

#include <chrono>
#include <mutex>
#include <condition_variable>
#include "raft_context.h"
#include "runnable.h"

namespace celestial {
class StepDownChecker : public Runnable {
public:
    StepDownChecker();
    void init();
    void run();
    void exit() {
        exiting_ = true;
    }
private:
    const int64_t ELECTION_TIMEOUT;
    bool exiting_;

    std::shared_ptr<RaftContext> raft_context_;
    std::shared_ptr<RaftManager> raft_manager_;

};
}

#endif
