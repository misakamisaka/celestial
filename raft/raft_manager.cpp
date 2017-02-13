#include <glog/logging.h>
#include "raft_manager.h"

namespace celestial {
void RaftManager::init() {
	election_checker_.reset(new ElectionTimeoutChecker());
}

/*
void RaftManager::appendEntries(AppendEntriesResponse& response,const AppendEntriesRequest& request){
	RaftContext* raft = RaftContext::GetInstance();
	std::lock_guard<std::mutex> lock(raft->context_mutex);
	response.term = raft->current_term;
	response.success = false;

	if(request.term < raft->current_term){
		return;
	}else if(request.term == raft->current_term){
		if(raft->state == LEADER){
			LOG(ERROR) << "one term has only one leader";
			return ;
		}else if(raft->state == CANDIDATE){
			//stepDown(request.term);
			//raft->leaderid = request.serverid();
			response.term = request.term;
		}
	}
	return ;
}*/




//candidate's term less than vote reponse , candidate step down to follower
void RaftManager::stepDown(int64_t term) {
	RaftContext *raft = RaftContext::GetInstance();
    if (raft->metadata->current_term < term) {
        raft->leader_id = 0;
        raft->metadata->voted_for = 0;
        raft->metadata->current_term = term;
        raft->metadata->save();
        raft->state = FOLLOWER;
    } else {
        raft->state = FOLLOWER;
    }
    resetElectionTimeout();
}
/*
void RaftManager::advanceCommitIndex() {
    if (raft_context_->state != LEADER) {
        return;
    }
    std::vector<int64_t> match_index_vec;
    for (auto& peer : raft_peers_) {
        match_index_vec.push_back(peer->get_match_index());
    }
    std::sort(match_index_vec.begin(), match_index_vec.end());
    raft_context_->commit_index = match_index_vec[(match_index_vec.size() - 1)/ 2];
}
*/
void RaftManager::resetElectionTimeout() {
    election_checker_->reset();
}
void RaftManager::startNewElection() {
	std::unique_lock<std::mutex> lock(RaftContext::GetInstance()->context_mutex);
	RaftContext* raft = RaftContext::GetInstance();

	raft->metadata->current_term++;
	raft->state = CANDIDATE;
	raft->leader_id = 0;
	raft->metadata->voted_for = raft->server_id;
	raft->metadata->save();
	LOG(INFO)<<"timeout, start new election leaderid["<<raft->leader_id<<"] , current term ["<<raft->metadata->current_term<<"], state ["<<raft->state<<"]";
	//resetElectionTimeout();
}
/*
bool RaftManager::checkStepDown() {
    return false;
}
*/

void RaftManager::checkVote() {
	RaftContext *raft = RaftContext::GetInstance();
    if (raft->state != CANDIDATE) {
        return;
    }
    std::vector<bool> have_vote_vec;
    for (auto& peer : raft_peers_) {
        have_vote_vec.push_back(peer->have_vote_);
    }
    std::sort(have_vote_vec.begin(), have_vote_vec.end());
    if(have_vote_vec[(have_vote_vec.size() - 1)/ 2]) {
		raft->state = LEADER;
		raft->leader_id = raft->server_id;
        election_checker_->setMax();
    }
}

/*
void RaftManager::append(const std::vector<shared_ptr<Entry> >& entries) {
    {
        std::lock_guard<std::mutex> lock(raft_context_->context_mutex);
        raft_context_->log->append(std::vector<Entry>(it, request.entries.end()));
    }
    
    for (auto & peer : raft_peers_) {
        peer->notify();
    }
}
*/
}
