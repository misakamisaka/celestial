#ifndef _CELESTIAL_RAFT_PEER_H_
#define _CELESTIAL_RAFT_PEER_H_

#include "runnable.h"
#include "raft_context.h"

namespace celestial {
	class RaftPeer : public Runnable {
public:    
	RaftPeer(){
		have_vote_ = false;
	}
	void run();
	bool have_vote_;
    void appendEntries();
/*
    int64_t get_match_index() {
        return match_index_;
    }

    void installSnapshot();
    void requestVote();

    void notify();
private:
    
    std::shared_ptr<RaftServiceClient> raft_service_client_;
    std::shared_ptr<RaftContext> raft_context_;
    std::shared_ptr<Snapshot> snapshot_;
    
    std::mutex peer_mutex;
    std::condition_variable peer_cond;

    int64_t next_index_;
    int64_t match_index_;


    int64_t server_id_;
    int64_t snapshot_offset_;

    const int64_t MAX_APPEND_ENTRY_NUM;
    const int64_t HEARTBEAT_INTERVAL;
	*/
};
}

#endif
