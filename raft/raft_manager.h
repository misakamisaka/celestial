#ifndef _CELESTIAL_RAFT_MANAGER_H_
#define _CELESTIAL_RAFT_MANAGER_H_
#include <memory>
#include "RaftService.h"
#include "raft_context.h"
#include "metadata.h"
#include "raft_peer.h"
#include "election_timeout_checker.h"

namespace celestial {
class RaftManager {
public:
	static RaftManager* GetInstance(){
		static RaftManager manager;
		return &manager;
	}
private:
	RaftManager(){
		init();
	};
	RaftManager& operator=(const RaftManager&)=delete;
	RaftManager(const RaftManager&)=delete;

public:
    void init();
	void appendEntries(AppendEntriesResponse& response,const AppendEntriesRequest& request);
	
    void stepDown(int64_t term);
    void startNewElection();
    std::vector<std::shared_ptr<RaftPeer> > raft_peers_;
    void checkVote();
    //std::unique_ptr<StepDownChecker> step_down_checker_;
	std::unique_ptr<ElectionTimeoutChecker> election_checker_;

    void resetElectionTimeout();
	/*
    bool checkStepDown();
    void advanceCommitIndex();

    void append(const std::vector<shared_ptr<Entry> >& entries);
private:
    //std::shared_ptr<RaftService> raft_service_;
    std::shared_ptr<RaftContext> raft_context_;
	*/
};
}

#endif
