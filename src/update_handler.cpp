#include "update_handler.h"
#include "global_variables.h"
#include <memory>
#include <future>
#include <glog/logging.h>

namespace task_manager {

//lock
//update collection
//if ok
//   save journal
//unlock

bool UpdateHandler::update(std::vector<int64_t>& delete_ids, std::vector<TaskPtr>& insert_tasks, const std::string& worker_name) {
    LOG(INFO) << "1111";
    Journal journal;
    journal.delete_taskids = delete_ids;
    for (auto& task_ptr : insert_tasks) {
        task_ptr->__set_task_id(++GlobalVariables::task_id);
        journal.insert_tasks.push_back(*task_ptr);
    }
    LOG(INFO) << "2222";

    bool ret = false;
    std::shared_ptr<std::promise<bool> > prom = std::make_shared<std::promise<bool> >();
    std::future<bool> fut = prom->get_future();

    LOG(INFO) << "3333";
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ret = task_collection_->update(delete_ids, insert_tasks, worker_name);
        LOG(INFO) << "35353535, ret:" << ret;
        if (ret) {
            checkpoint_manager_->addToQueue(std::make_pair(journal, prom));
        }
    LOG(INFO) << "4444";
    }

    if (ret) {
        ret = fut.get();
    }

    LOG(INFO) << "5555, ret:" << ret;
    return ret;
}
}
