#ifndef _CELESTIAL_RAFT_CONTEXT_H_
#define _CELESTIAL_RAFT_CONTEXT_H_

#include <memory>
#include <mutex>
#include "log.h"
#include "snapshot.h"

namespace celestial {

enum RaftState {
    FOLLOWER,
    CANDIDATE,
    LEADER
};

struct RaftContext {
public:
    std::mutex context_mutex;
    std::condition_variable context_cond;
    std::shared_ptr<Log> log;
    std::shared_ptr<Snapshot> snapshot;
    std::shared_ptr<Meatadata> metadata;
    int64_t commit_index;

    int64_t current_term;
    int64_t voted_for;
    int64_t serverid;
    int64_t leaderid;
    RaftState state;
};
}
#endif
