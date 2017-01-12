#include "raft_manager.h"

namespace celestial {
void RaftManager::init() {
}
void RaftManager::stepDown(int64_t term) {
    if (raft_context_->current_term < term) {
        raft_context_->leaderid = 0;
        raft_context_->metadata->voted_for = 0;
        raft_context_->metadata->current_term = term;
        raft_context_->metadata->save();
        raft_context_->state = FOLLOWER;
    } else {
        raft_context_->state = FOLLOWER;
    }
    
    resetElectionTimeout();
}

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

void resetElectionTimeout() {
    election_checker_->reset();
}
void RaftManager::startNewElection() {
    std::unique_lock<std::mutex> lock(raft_context_->context_mutex);
    ++raft_context_->metadata->current_term;
    LOG(INFO) << "timeout, start new election, leaderid[" << raft_context_->leaderid <<
        "], current term[" << ++raft_context_->metadata->current_term << "], state[" <<
        raft_context_->state << "]";
    raft_context_->state = CANDIDATE;
    raft_context_->leader_id = 0;
    raft_context_->metadata->voted_for = raft_context_->server_id;
    
    raft_context_->metadata->save();
    resetElectionTimeout();
}

bool RaftManager::checkStepDown() {
    return false;
}
void RaftManager::checkVote() {
    if (raft_context_->state != CANDIDATE) {
        return;
    }
    std::vector<bool> have_vote_vec;
    for (auto& peer : raft_peers_) {
        have_vote_vec.push_back(peer->have_vote());
    }
    std::sort(have_vote_vec.begin(), have_vote_vec.end());
    if(have_vote_vec[(have_vote_vec.size() - 1)/ 2]) {
        raft_context_->state = LEADER;
        raft_context_->leader_id = raft_context_->server_id;
        election_checker_->setMax();
    }
}
void RaftManager::append(const std::vector<shared_ptr<Entry> >& entries) {
    {
        std::lock_guard<std::mutex> lock(raft_context_->context_mutex);
        raft_context_->log->append(std::vector<Entry>(it, request.entries.end()));
    }
    
    for (auto & peer : raft_peers_) {
        peer->notify();
    }
}
}
