#define GTEST_USE_OWN_TR1_TUPLE 0

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <memory>
#include "log.h"
#include "log_exception.h"
#include "raft_service_types.h"
#include <glog/logging.h>

#include "raft_context.h"
#include "raft_manager.h"
#include "election_timeout_checker.h"
//#include "raft_service.h"

#undef private
#undef protected

using namespace std;
using namespace celestial;

class RaftPeerTest : public testing::Test {
public:
    RaftPeerTest() {
    }
protected:
    virtual void SetUp() {
    }
public:
};
TEST_F(RaftPeerTest,test_service_init){
	
	RaftContext *raft = RaftContext::GetInstance();
	RaftManager *manager = RaftManager::GetInstance();
	/*
	AppendEntriesRequest request;
	AppendEntriesResponse response;
	manager->appendEntries(response,request);
	manager->heartBeatTimer();
	*/
	
	ElectionTimeoutChecker* checker = new ElectionTimeoutChecker();
	checker->reset();
	checker->start();

	std::this_thread::sleep_for(std::chrono::seconds(3));
	ASSERT_EQ(raft->metadata->current_term,1);
	ASSERT_EQ(raft->state,CANDIDATE);
	return ;
}
TEST_F(RaftPeerTest, test_RaftManager_checkVote){

	RaftContext *raft = RaftContext::GetInstance();
	RaftManager *manager = RaftManager::GetInstance();
	
	//first unitest
	raft->state = CANDIDATE;
	std::shared_ptr<RaftPeer> p1 = make_shared<RaftPeer>();
	p1->have_vote_ = true;
	manager->raft_peers_.push_back(p1);
	manager->checkVote();
	ASSERT_EQ(raft->state,LEADER);
	//second unitest
	raft->state = CANDIDATE;
	std::shared_ptr<RaftPeer> p2 = make_shared<RaftPeer>();
	p2->have_vote_ = false;
	manager->raft_peers_.push_back(p2);

	std::shared_ptr<RaftPeer> p3 = make_shared<RaftPeer>();
	p3->have_vote_ = false;
	manager->raft_peers_.push_back(p3);
	manager->checkVote();
	ASSERT_EQ(raft->state,CANDIDATE);
}


TEST_F(RaftPeerTest,test_RaftManager_stepDown){
	RaftContext *raft = RaftContext::GetInstance();
	RaftManager *manager = RaftManager::GetInstance();
	manager->stepDown(5);

	ASSERT_EQ(raft->metadata->current_term,5);
	ASSERT_EQ(raft->state,FOLLOWER);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
