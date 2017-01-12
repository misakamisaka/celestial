#include "raft_service.h"

namespace celestial {

void RaftServiceHandler::appendEntries(AppendEntriesResponse& repsonse, const AppendEntriesRequest& request) {
    std::lock_gurad<std::mutex> lock(raft_context_->context_mutex);
    repsonse.term = raft_context_->current_term;
    repsonse.success = false;

    if (request.term < raft_context_->current_term) {
        return;
    } else if (request.term == raft_context_->current_term) {
        if (raft_context_->state == LEADER) {
            LOG(ERROR) << "one term has only one leader";
            return;
        } else if (raft_context_->state == CANDIDATE) {
            raft_manager_->stepDown(request.term);
            raft_context_->leaderid = request.serverid();
        }
    } else {
        raft_manager_->stepDown(request.term);
        raft_context_->leaderid = request.serverid();
        repsonse.term = request.term;
    }

    if (request.prev_log_index > raft_context_->log->getLastIndex()){
        LOG(INFO) << "prev_log_index[" << request.prev_log_index <<
            "] not match, log last index[" << raft_context_->log->getLastIndex() <<"]";
        return;
    }
    if (request.prev_log_index >= raft_context_->log->getStartIndex() &&
        raft_context_->log->getEntryFromIndex(request.prev_log_index).term != request.prev_log_term) {
        LOG(INFO) << "prev_log_index[" << request.prev_log_index <<
            "], prev_log_term[" << request.prev_log_term << 
            "] not match, term[" << raft_context_->log->getEntryFromIndex(request.prev_log_index).term <<"]";
        return;
    }
    repsonse.sucess = true;
    
    int64_t index = request.prev_log_index;
    auto it = request.entries.begin();
    for (; it != request.entries.end(); ++it) {
        ++index;
        if (index < raft_context_->log->getStartIndex()) {
            continue;
        }
        if (index <= raft_context_->log->getLastIndex()) {
            if (raft_context_->log->getEntryFromIndex(index).term == it->term) {
                continue;
            }
            raft_context_->log->truncateSuffix(index - 1);
            break;
        }
    }
    raft_context_->log->append(std::vector<Entry>(it, request.entries.end()));
    raft_context_->commit_index = std::min<int64_t>(request.commit_index,
            raft_context_->log->getLastIndex());
    raft_manager_->resetElectionTimeout();

}
void RaftServiceHandler::installSnapshot(InstallSnapshotResponse& repsonse, const InstallSnapshotRequest& request) {
    std::lock_gurad<std::mutex> lock(raft_context_->context_mutex);
    repsonse.term = raft_context_->current_term;

    if (request.term < raft_context_->current_term) {
        return;
    } else if (request.term == raft_context_->current_term) {
        if (raft_context_->state == LEADER) {
            LOG(ERROR) << "one term has only one leader";
            return;
        } else if (raft_context_->state == CANDIDATE) {
            raft_manager_->stepDown(request.term);
            raft_context_->leaderid = request.serverid();
        }
    } else {
        raft_manager_->stepDown(request.term);
        raft_context_->leaderid = request.serverid();
        repsonse.term = request.term;
    }
    if (request.offset == 0 ||
            snapshot_->getLastIncludedIndex() != request.last_included_index ||
            snapshot_->getLastIncludedTerm() != request.last_included_term) {
        snapshot_ = make_shared<Snapshot>();
        snapshot_->setLastIncludedIndex(request.last_included_index);
        snapshot_->setLastIncludedTerm(request.last_included_term);
        snapshot_offset_ = 0;
    }
    if (request.offset > snapshot_offset_) {
        repsonse.bytes_stored = snapshot_offset_;
        return;
    }
    snapshot_->writeDataFromOffset(request.data, request.offset);
    snapshot_offset_ += request.data.size();
    repsonse.bytes_stored = snapshot_offset_;
    if (request.done) {
        SnapshotManager::instance()->add(snapshot_offset_);
        //TODO:state machine load snapshot here
    }
    
    
}
void RaftServiceHandler::requestVote(RequestVoteReponse& repsonse, const RequestVoteRequest& request) {
    std::lock_guard<std::mutex> lock(raft_context_->context_mutex);
    repsonse.term = raft_context_->current_term;
    repsonse.granted = false;
    
    if (request.term < raft_context_->current_term) {
        return;
    } else if (request.term == raft_context_->current_term) {
        if (raft_context_->state != FOLLOWER) {
            return;
        } 
    } else {
        raft_manager_->stepDown(request.term);
    }

    if (raft_context_->voted_for == 0 || raft_context_->voted_for == serverid) {
        int64_t last_index = raft_context_->log->getLastIndex();
        int64_t last_term = raft_context_->log->getEntryFromIndex(last_index).term;
        if ((request.last_log_term == last_term && request.last_log_index >= last_index) ||
                request.last_log_term > last_term) {
            repsonse.granted = true;
            if (raft_context_->voted_for == 0) {
                raft_context_->voted_for = serverid;
            }
        }
    }
    repsonse.term = raft_context_->current_term;
    raft_manager_->resetElectionTimeout();
}
}
