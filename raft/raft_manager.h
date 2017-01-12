#ifndef _CELESTIAL_RAFT_MANAGER_H_
#define _CELESTIAL_RAFT_MANAGER_H_

namespace celestial {
class RaftManager {
public:
    void init();
    void stepDown(int64_t term);
    void startNewElection();
    bool checkStepDown();
    void checkVote();

    void advanceCommitIndex();
    void resetElectionTimeout();

    void append(const std::vector<shared_ptr<Entry> >& entries);
private:
    std::shared_ptr<RaftService> raft_service_;
    std::shared_ptr<RaftContext> raft_context_;
    std::vector<std::shared_ptr<RaftPeer> > raft_peers_;
    std::unique_ptr<ElectionTimeoutChecker> election_checker_;
    //std::unique_ptr<StepDownChecker> step_down_checker_;
};
}

#endif
