#include <gtest/gtest.h>
#include "raft_service.h"
#include "configure.h"
#include "log_exception.h"
using namespace celestial;
using namespace std;
class RaftServiceTest : public testing::Test{
public:
	RaftServiceTest(){

	}
protected:
	virtual void SetUp(){
		Json::Value root;
		root["log_dir"] = ".";
		root["log_prefix"] = "log.";
		root["log_roll_size"] = 200;
		Json::FastWriter writer;
		Configure::instance()->init(writer.write(root));
	}
};
TEST_F(RaftServiceTest,test_requestVote){
	
	RaftContext* raft = RaftContext::GetInstance();
	
	{
	RequestVoteRequest request;
	RequestVoteResponse response;
	try{
		request.serverid = 3;
		request.term = 1;
		request.last_log_index = raft->log->getLastIndex();
		request.last_log_term = 0;
		requestVote_callback(response,request);
	}catch(LogException ex){
		std::cout<<ex.what()<<std::endl;
	}

	ASSERT_EQ(response.granted,true);
	ASSERT_EQ(response.term,1);

	}


	{
		RequestVoteRequest request;
		RequestVoteResponse response;
		
		raft->metadata->current_term = 1;

		request.serverid = 3;
		request.term = 1;
		request.last_log_index = 5;
		request.last_log_term = 1;
		requestVote_callback(response,request);

		ASSERT_EQ(response.granted,true);
		ASSERT_EQ(response.term, 1);
	}



	{
		raft->metadata->current_term = 1;
		Log log;
		shared_ptr<Entry> entry = make_shared<Entry>();
		entry->term = 1;
		entry->entry_type = EntryType::DATA;
		entry->data = "aaa";
		vector<shared_ptr<Entry> > entries({entry});
		log.append(entries);

		
		RequestVoteRequest request;
		RequestVoteResponse response;
		request.serverid = 3;
		request.term = 1;
		request.last_log_index = 0;
		request.last_log_term = 0;
		requestVote_callback(response,request);

		ASSERT_EQ(response.granted,false);
		ASSERT_EQ(response.term,1);
	}
}

int main(int argc,char *argv[]){
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
