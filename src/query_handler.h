#ifndef QUERY_HANDLER_H
#define QUERY_HANDLER_H

#include "task_collection.h"
#include "task_manager_types.h"

namespace task_manager {
class QueryHandler {
public:
    void init(std::shared_ptr<TaskCollection> task_collection) {
        task_collection_ = task_collection;
    }
    void query(std::string group_name,
        int32_t limit_num,
        OwnershipMode::type owner_mode,
        PickUpPolicy::type policy,
        std::vector<TaskInfo>& tasks);
private:
    std::shared_ptr<TaskCollection> task_collection_;
};
}

#endif //QUERY_HANDLER_H
