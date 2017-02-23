#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "log.h"
#include "log_exception.h"
#include "raft_service_types.h"
#include "configure.h"
#include <glog/logging.h>
#undef private
#undef protected

using std::vector;
using std::string;
using std::shared_ptr;
using namespace celestial;

class LogTest : public testing::Test {
public:
    LogTest() {
    }
protected:
    virtual void SetUp() {
    }
public:
};

TEST_F(LogTest, test_log) {
    Json::Value root;
    root["log_dir"] = ".";
    root["log_prefix"] = "log.";
    root["log_roll_size"] = 200;
    Json::FastWriter writer;
	
    Configure::instance()->init(writer.write(root));
	
	Configure* config = Configure::instance();
	
	
	{
		Log log;
		vector<shared_ptr<Entry> > entry1;
		uint32_t x = 1;
		log.getEntryFromIndex(1, x, entry1);
	}
	
    {
        Log log;
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 1;
            entry->entry_type = EntryType::DATA;
            entry->data = "aaa";
            vector<shared_ptr<Entry> > entries({entry});
            log.append(entries);
        }
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 2;
            entry->entry_type = EntryType::DATA;
            entry->data = "bbbb";
            vector<shared_ptr<Entry> > entries({entry});
            log.append(entries);
        }
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 3;
            entry->entry_type = EntryType::NOOP;
            entry->data = "ccccc";
            vector<shared_ptr<Entry> > entries({entry});
            log.append(entries);
        }
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 4;
            entry->entry_type = EntryType::DATA;
            entry->data = "dddddd";
            vector<shared_ptr<Entry> > entries({entry});
            log.append(entries);
        }

		for (int i = 0 ;i<10;i++){
			shared_ptr<Entry> entry = std::make_shared<Entry>();
			entry->term = 4;
			entry->entry_type = EntryType::DATA;
			entry->data = "eeee";
			vector<shared_ptr<Entry> > entries({entry});
			log.append(entries);
		}
    }
	
	
    {
        Log log;
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 1;
            log.getEntryFromIndex(1, x, entry1);
            ASSERT_EQ(entry1[0]->term, 1);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
            ASSERT_EQ(entry1[0]->data, "aaa");
        }
		try{
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 1;
            log.getEntryFromIndex(3, x, entry1);
            ASSERT_EQ(entry1[0]->term, 3);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::NOOP);
            ASSERT_EQ(entry1[0]->data, "ccccc");
        }   
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 3;
            log.getEntryFromIndex(7, x, entry1);
            ASSERT_EQ(entry1[0]->term, 4);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
            ASSERT_EQ(entry1[0]->data, "eeee");
            ASSERT_EQ(entry1[1]->data, "eeee");
            ASSERT_EQ(entry1[2]->data, "eeee");
        }
		}catch(LogException ex){
			std::cout<<ex.what()<<std::endl;
		}

	}
	
    {
        Log log;
        log.truncateFromIndex(7);
        vector<shared_ptr<Entry> > entry1;
        uint32_t x = 100;
        log.getEntryFromIndex(2, x, entry1);
		
        ASSERT_EQ(x, 5);
        ASSERT_EQ(entry1.size(), 5);
        ASSERT_EQ(entry1[0]->term, 2);
        ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[0]->data, "bbbb");
        
		shared_ptr<Entry> entry5 = std::make_shared<Entry>();
        entry5->term = 5;
        entry5->entry_type = EntryType::DATA;
        entry5->data = "eeeeeeee";
        log.append({entry5});
        entry1.clear();
        x = 100;
        log.getEntryFromIndex(7, x, entry1);
        ASSERT_EQ(entry1[0]->term, 5);
        ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[0]->data, "eeeeeeee");
		ASSERT_EQ(x,1);
		try{
			uint32_t x = 1;
			vector<shared_ptr<Entry> > entry1;
			log.getEntryFromIndex(12312313,x,entry1);
		}catch(LogException &e){
			ASSERT_STREQ(e.what(),"cannot find index[12312313],start_index[7],position_vec.size[2]");
		}
    }
	/*
    {
        Log log;
        log.truncateFromIndex(1111124);
        vector<shared_ptr<Entry> > entry1;
        uint32_t x = 100;
        log.getEntryFromIndex(1111123, x, entry1);
        ASSERT_EQ(x, 1);
        ASSERT_EQ(entry1.size(), 1);
        ASSERT_EQ(entry1[0]->term, 1);
        ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[0]->data, "aaa");
        try {
            log.getEntryFromIndex(1111123, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[1111123]");
        }
        try {
            log.getEntryFromIndex(1111124, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[1111124]");
        }
        try {
            log.getEntryFromIndex(1111125, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[1111125]");
        }
        try {
            log.getEntryFromIndex(102, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[102]");
        }
        log.truncateFromIndex(1111123);
        
    }
	*/
	
	
	
	//clear all log file
	namespace fs = boost::filesystem;
	fs::path filepath(config->get_log_dir());
	fs::directory_iterator endIterator;
	for(fs::directory_iterator fileIterator(filepath);
				fileIterator != endIterator; ++fileIterator){
		std::string filename = fileIterator->path().filename().string();
		if(boost::algorithm::starts_with(filename,config->get_log_prefix()))
		{
			fs::path deletefile = config->get_log_dir()+"/"+filename;
			remove(deletefile);
		}
	}
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
