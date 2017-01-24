#ifndef _CELESTIAL_UPDATE_HANDLER_H_
#define _CELESTIAL_UPDATE_HANDLER_H_

#include <mutex>
#include "task_collection.h"
#include "base_checkpoint_manager.h"

namespace task_manager {
class UpdateHandler {
public:
    void init(std::shared_ptr<TaskCollection> task_collection) {
        task_collection_ = task_collection;
    }
    bool update(std::vector<int64_t>& delete_ids, std::vector<TaskPtr>& insert_tasks, const std::string& worker_name);
private:
    std::mutex mutex_;
    std::shared_ptr<TaskCollection> task_collection_;
};
}

#endif //_CELESTIAL_UPDATE_HANDLER_H_
