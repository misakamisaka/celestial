#include "raft_peer.h"
#include <chrono>
#include "RaftService.h"

using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::duration;
using apache::thrift::TException;

namespace celestial {

void RaftPeer::run() {
	/*
    while(true) {
        std::unique_lock<std::mutex> lock(peer_mutex_);
        time_point<system_clock> start_time = system_clock::now();
        duration<int64_t, std::microseconds> wait_time = HEARTBEAT_INTERVAL;
        switch (raft_context_->state) {
            case LEADER:
                appendEntries();
                break;
            case CANDIDATE:
                requestVote();
                break;
            case FOLLOWER:
                break;
        }
        peer_cond_.wait_until(lock, start_time + wait_time);
    }
	*/
}
/*
void notify() {
    std::unique_lock<std::mutex> lock(peer_mutex_);
    peer_cond_.notify_all();
}

void RaftPeer::appendEntries() {
    int64_t prev_log_index = next_index_ - 1;

    if (prev_log_index < raft_context_->log->getStartIndex()) {
        installSnapshot();
        return;
    }

    AppendEntriesRequest request;

    {
        std::unique_lock<std::mutex> lock(raft_context_->context_mutex);
        request.serverid = raft_context_->serverid;
        request.term = raft_context_->current_term;
        request.prev_log_index = prev_log_index;
        shared_ptr<Entry> prev_entry = raft_context_->log->getEntryFromIndex(prev_log_index);
        request.prev_log_term = prev_entry->term;

        int64_t end_index = std::min<int64_t>(raft_context_->log->getLastIndex(),
                prev_log_index + MAX_APPEND_ENTRY_NUM);
        for (int64_t i = next_index_; i <= end_index; ++i) {
            shared_ptr<Entry> entry = raft_context_->log->getEntryFromIndex(i);
            request.entries.push_back(std::move(*entry));
        }
        request.commit_index = raft_context_->commit_index;
    }

    AppendEntriesResponse resonpse;

    try {
        raft_service_client_->appendEntries(resonpse, request);
    } catch (apache::thrift::TException& e) {
        LOG(ERROR) << "error occurred when append entries, error:[" << e.what() << "]";
    }

    {
        std::unique_lock<std::mutex> lock(raft_context_->context_mutex);
        if (raft_context_->current_term != request.term) {
            return;
        }
        if (raft_context_->current_term < response.term) {
            raft_manager_->stepDown(response.term);
        } else {
            if (reponse.success) {
                if (match_index_ < end_index) {
                    match_index_ = end_index
                } else {
                    LOG(WARNING) << "match index bigger than end_index";
                }
                raft_manager_->advanceCommitIndex();
            } else {
                if (next_index_ > 1) {
                    --next_index_;
                }
                if (response.__isset.last_log_index &&
                        next_index_ > response.last_log_index + 1) {
                    next_index_ = response.last_log_index + 1;
                }
            }
        }
    }
}

void RaftPeer::installSnapshot() {
    InstallSnapshotRequest request;

    {
        std::unique_lock<std::mutex> lock(raft_context_->context_mutex);
        request.serverid = raft_context_->serverid;
        request.term = raft_context_->current_term;
        if (!snapshot_) {
            snapshot_ = raft_context_->snapshot_manager->get_snapshot();
            snapshot_offset_ = 0;
        }
        request.last_included_index = snapshot_->getLastIncludedIndex();
        request.last_included_term = snapshot_->getLastIncludedTerm();
        request.offset = snapshot_offset_;
        request.data = snapshot_->readDataFromOffset(snapshot_offset_);
        request.done = (snapshot_->getFileSize() == snapshot_offset_ + request.data.size());
    }

    AppendEntriesResponse resonpse;

    try {
        raft_service_client_->appendEntries(resonpse, request);
    } catch (apache::thrift::TException& e) {
        LOG(ERROR) << "error occurred when append entries, error:[" << e.what() << "]";
    }

    {
        std::unique_lock<std::mutex> lock(raft_context_->context_mutex);
        if (raft_context_->current_term != request.term) {
            return;
        }
        if (raft_context_->current_term < response.term) {
            raft_manager_->stepDown(response.term);
        } else {
            snapshot_offset_ = reponse.bytes_stored;
            if (snapshot_offset_ == snapshot_->getFileSize()) {
                snapshot_.reset();
                snapshot_offset_ = 0;
            }
        }
    }
}

void RaftPeer::requestVote() {
    RequestVoteRequest request;
	RaftContext *raft = RaftContext::GetInstance();
	{
		std::unique_lock<std::mutex> lock(raft->context_mutex);
		request.serverid = raft->server_id;
		request.term = raft->metadata->current_term;
		//request.last_log_index = raft->log->getLastIndex();
        //request.last_log_term = raft_context_->log->getEntryFromIndex(request.last_log_index)->term;
    }

    RequestVoteResponse response;

    try {
        raft_service_client_->requestVote(response, request);
    } catch( TException& e) {
        LOG(ERROR) << "error occurred when requestVote, error:[" << e.what() << "]";
    }

    {
        std::unique_lock<std::mutex> lock(raft_context_->context_mutex);
        if (raft_context_->current_term != request.term) {
            return;
        }
        if (raft_context_->current_term < response.term) {
			RaftManager::GetInstance()->stepDown(response.term);
        } else {
            if (response.granted) {
                have_vote_ = true;
				RaftManager::GetInstance()->checkVote();
            } else {
				//deny 
                LOG(INFO) << "didn't get vote from[" << server_id_ << "]";
            }
        }
    }
}
*/
}
