#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "log_file.h"
#include "log_exception.h"
#include "raft_service_types.h"
#undef private
#undef protected

using std::vector;
using std::string;
using std::shared_ptr;
using namespace celestial;

class LogFileTest : public testing::Test {
public:
    LogFileTest() {
    }
protected:
    virtual void SetUp() {
    }
public:
};

TEST_F(LogFileTest, test_log_file) {
    std::string log_path = "./log_file.1111123";
    {
        LogFile log_file(log_path);
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 1;
            entry->entry_type = EntryType::DATA;
            entry->data = "aaa";
            vector<shared_ptr<Entry> > entries({entry});
            log_file.append(entries);
        }
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 2;
            entry->entry_type = EntryType::DATA;
            entry->data = "bbbb";
            vector<shared_ptr<Entry> > entries({entry});
            log_file.append(entries);
        }
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 3;
            entry->entry_type = EntryType::NOOP;
            entry->data = "ccccc";
            vector<shared_ptr<Entry> > entries({entry});
            log_file.append(entries);
        }
        {
            shared_ptr<Entry> entry = std::make_shared<Entry>();
            entry->term = 4;
            entry->entry_type = EntryType::DATA;
            entry->data = "dddddd";
            vector<shared_ptr<Entry> > entries({entry});
            log_file.append(entries);
        }
    }
    {
        LogFile log_file(log_path);
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 1;
            log_file.getEntryFromIndex(1111126, x, entry1);
            ASSERT_EQ(entry1[0]->term, 4);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
            ASSERT_EQ(entry1[0]->data, "dddddd");
        }
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 1;
            log_file.getEntryFromIndex(1111123, x, entry1);
            ASSERT_EQ(entry1[0]->term, 1);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
            ASSERT_EQ(entry1[0]->data, "aaa");
        }
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 1;
            log_file.getEntryFromIndex(1111124, x, entry1);
            ASSERT_EQ(entry1[0]->term, 2);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
            ASSERT_EQ(entry1[0]->data, "bbbb");
        }
        {
            vector<shared_ptr<Entry> > entry1;
            uint32_t x = 1;
            log_file.getEntryFromIndex(1111125, x, entry1);
            ASSERT_EQ(entry1[0]->term, 3);
            ASSERT_EQ(entry1[0]->entry_type, EntryType::NOOP);
            ASSERT_EQ(entry1[0]->data, "ccccc");
        }
        
    }
    {
        LogFile log_file(log_path);
        log_file.truncateFromIndex(1111126);
        vector<shared_ptr<Entry> > entry1;
        uint32_t x = 100;
        log_file.getEntryFromIndex(1111123, x, entry1);
        ASSERT_EQ(x, 3);
        ASSERT_EQ(entry1.size(), 3);
        ASSERT_EQ(entry1[0]->term, 1);
        ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[0]->data, "aaa");
        ASSERT_EQ(entry1[1]->term, 2);
        ASSERT_EQ(entry1[1]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[1]->data, "bbbb");
        ASSERT_EQ(entry1[2]->term, 3);
        ASSERT_EQ(entry1[2]->entry_type, EntryType::NOOP);
        ASSERT_EQ(entry1[2]->data, "ccccc");
        shared_ptr<Entry> entry5 = std::make_shared<Entry>();
        entry5->term = 5;
        entry5->entry_type = EntryType::DATA;
        entry5->data = "eeeeeeee";
        log_file.append({entry5});
        entry1.clear();
        x = 1;
        log_file.getEntryFromIndex(1111126, x, entry1);
        ASSERT_EQ(entry1[0]->term, 5);
        ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[0]->data, "eeeeeeee");
        
    }

    {
        LogFile log_file(log_path);
        log_file.setDelete();
        log_file.truncateFromIndex(1111124);
        vector<shared_ptr<Entry> > entry1;
        uint32_t x = 100;
        log_file.getEntryFromIndex(1111123, x, entry1);
        ASSERT_EQ(x, 1);
        ASSERT_EQ(entry1.size(), 1);
        ASSERT_EQ(entry1[0]->term, 1);
        ASSERT_EQ(entry1[0]->entry_type, EntryType::DATA);
        ASSERT_EQ(entry1[0]->data, "aaa");
        try {
            log_file.getEntryFromIndex(1111123, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[1111123]");
        }
        try {
            log_file.getEntryFromIndex(1111124, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[1111124]");
        }
        try {
            log_file.getEntryFromIndex(1111125, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[1111125]");
        }
        try {
            log_file.getEntryFromIndex(102, x, entry1);
        } catch (LogException &e) {
            ASSERT_STREQ(e.what(), "cannot find index[102]");
        }
        
    }

}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
