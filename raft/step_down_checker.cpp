#include "step_down_checker.h"

namespace celestial {
StepDownChecker::StepDownChecker()
    : ELECTION_TIMEOUT(Configure["election_timeout"].asint64()),
    exiting_(false) {
}

void StepDownChecker::run() {
    using namespace std::chrono;
    while(!exiting_) {
        if (raft_context_->state == LEADER && raft_manager_->checkStepDown()) {
            raft_manager_->stepDown(raft_context_->current_term);
        }
        usleep(ELECTION_TIMEOUT / 1000);
    }
}
}
