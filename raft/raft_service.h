#ifndef _CELESTIAL_RAFT_SERVICE_H_
#define _CELESTIAL_RAFT_SERVICE_H_

#include "RaftService.h"
#include <thread>
#include <memory>

#include "raft_context.h"
#include "raft_manager.h"

namespace celestial {

void requestVote_callback(RequestVoteResponse& response,const RequestVoteRequest& request);

class RaftService : virtual public RaftServiceIf {
public:
    RaftService();
    void init();

    void appendEntries(AppendEntriesResponse& repsonse, const AppendEntriesRequest& request);
    void installSnapshot(InstallSnapshotResponse& repsonse, const InstallSnapshotRequest& request);
    void requestVote(RequestVoteResponse& repsonse, const RequestVoteRequest& request);

    //periodically check if quorum is met, advance commit index. only leader.
    //void checkQuorum();
private:
    std::shared_ptr<RaftContext> raft_context_;
    std::shared_ptr<RaftManager> raft_manager_;
    //std::shared_ptr<Snapshot> snapshot_;
};
}

#endif
