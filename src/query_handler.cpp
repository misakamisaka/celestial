#include "query_handler.h"
#include <limits>
#include <glog/logging.h>

namespace task_manager {
void QueryHandler::query(std::string group_name,
        int32_t limit_num,
        OwnershipMode::type owner_mode,
        PickUpPolicy::type policy,
        std::vector<TaskInfo>& tasks) {
    std::vector<TaskPtr> taskptr_vec;
    task_collection_->getTasksInGroup(group_name, taskptr_vec);
    
    auto end_it = taskptr_vec.end();
    if (owner_mode == OwnershipMode::ExcludeOwned) {
        end_it = std::remove_if(taskptr_vec.begin(), taskptr_vec.end(),
                                    [](TaskPtr& a) { 
                                        return !a->owner.empty();
                                    });
    }

    if (policy == PickUpPolicy::OLDEST) {
        std::sort(taskptr_vec.begin(), end_it, [](const TaskPtr& a, const TaskPtr& b) {
            return a->creation_time < b->creation_time;
        });
    } else if (policy == PickUpPolicy::RANDOM){
        std::random_shuffle(taskptr_vec.begin(), end_it);
    }
    
    int i = 0;
    limit_num = (limit_num > 0) ? limit_num : (std::numeric_limits<int>::max());
    for (auto it = taskptr_vec.begin(); it != end_it && i < limit_num; ++it, ++i) {
        tasks.push_back(*(*it));
    }
}
}
