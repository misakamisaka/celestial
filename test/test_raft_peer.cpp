#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "log.h"
#include "log_exception.h"
#include "raft_service_types.h"
#include <glog/logging.h>
#undef private
#undef protected

using std::vector;
using std::string;
using std::shared_ptr;
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

TEST_F(RaftPeerTest, test_peer) {

}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
