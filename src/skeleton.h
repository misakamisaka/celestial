#ifndef _CELESTIAL_SKELETON_H_
#define _CELESTIAL_SKELETON_H_

#include <memory>
#include <json/json.h>
#include <boost/shared_ptr.hpp>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/concurrency/ThreadManager.h>
#include "eta_checker.h"

namespace task_manager {
class  Skeleton {
public:
    void init();
    void run();
private:
    int port_;
    std::shared_ptr<ETAChecker> eta_checker_;
    std::shared_ptr<BaseCheckpointManager> checkpoint_manager_;
    std::shared_ptr<apache::thrift::server::TThreadPoolServer> server_;
};
}

#endif //_CELESTIAL_SKELETON_H_
