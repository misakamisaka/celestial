#ifndef _CELESTIAL_RAFT_CONTEXT_H_
#define _CELESTIAL_RAFT_CONTEXT_H_

#include <memory>
#include <mutex>
#include <map>
#include <utility>
#include "metadata.h"
//#include "log.h"
//#include "snapshot.h"

namespace celestial {

enum RaftState {
    FOLLOWER,
    CANDIDATE,
    LEADER
};
class RaftContext {
public:
	static RaftContext* GetInstance(){
		static RaftContext raft;
		return &raft;
	}
private:
	RaftContext(){
		state = FOLLOWER;
		metadata=std::make_shared<MetaData>();
		leader_id = 0;
		server_id = 0;
	};
	RaftContext& operator=(const RaftContext&);
	RaftContext(const RaftContext&)=delete;

public:
    std::mutex context_mutex;
    //std::condition_variable context_cond;
    //std::shared_ptr<Log> log;
    //std::shared_ptr<SnapshotManager> snapshot_manager;
    std::shared_ptr<MetaData> metadata;
    int64_t commit_index;

    int64_t server_id;
    int64_t leader_id;
    RaftState state;
	
	//int64_t myNodeID;
	//int nodeNum;
	
};
}
#endif
