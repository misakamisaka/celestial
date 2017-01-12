#include "task_collection.h"
#include <glog/logging.h>

using std::list;
using std::string;

namespace task_manager {
//TaskCollection::TaskCollection(Checkpoint& checkpoint) {
//    for (auto& group : checkpoint.groups) {
//        for (auto& task: group.tasks) {
//            list<TaskPtr>& task_list = groups_map_[group.name];
//            int64_t task_id = task.task_id;
//            auto it = task_list.insert(task_list.end(), std::make_shared<TaskInfo>(std::move(task)));
//            tasks_map_.insert(std::pair<int64_t, list<TaskPtr>::iterator>(task_id, it)); 
//        }
//    }
//}
//
//void TaskCollection::getTasksInGroup(const std::string& group_name, std::vector<TaskPtr>& tasks) {
//    std::list<TaskPtr>& tasks_list = groups_map_[group_name];
//    tasks.assign(tasks_list.begin(), tasks_list.end());
//}
//
//bool TaskCollection::update(std::vector<int64_t>& delete_ids, std::vector<TaskPtr>& insert_tasks, const std::string& worker_name){
//    LOG(INFO) << "begin update";
//    for (auto& id : delete_ids) {
//        LOG(INFO) << "delete id:" << id;
//        auto it = tasks_map_.find(id);
//        if (it == tasks_map_.end()) {
//            LOG(INFO) << "id[" << id << "] not exist";
//            return false;
//        } else if (!worker_name.empty() &&
//                !(*(it->second))->owner.empty() &&
//                (*(it->second))->owner != worker_name) {
//            LOG(INFO) << "id[" << id << "] owner:" << (*(it->second))->owner << ", worker_name:" << worker_name;
//            return false;
//        }
//    }
//
//    LOG(INFO) << "delete check over";;
//    for (auto& task_ptr : insert_tasks) {
//        list<TaskPtr>& task_list = groups_map_[task_ptr->group];
//        auto it = task_list.insert(task_list.end(), task_ptr);
//        tasks_map_.insert(std::pair<int64_t, list<TaskPtr>::iterator>(task_ptr->task_id, it)); 
//    }
//    LOG(INFO) << "insert over";;
//
//    for (auto& id : delete_ids) {
//        LOG(INFO) << "delete:" << id;
//        auto it = tasks_map_[id];
//        LOG(INFO) << "before delete group:" << (*it)->group << ", size:" << groups_map_[(*it)->group].size();
//        groups_map_[(*it)->group].erase(it);
//        
//        LOG(INFO) << "after delete group:" << (*it)->group << ", size:" << groups_map_[(*it)->group].size();
//    }
//    LOG(INFO) << "delete over";;
//    
//    return true;
//}
//
//void TaskCollection::getAll(std::list<TaskPtr>& tasks) {
//    for (auto& pair : groups_map_) {
//        
//        LOG(INFO) << "group:" << pair.first << ", size:" << pair.second.size();
//        LOG(INFO) << "before splice tasks.size:" << tasks.size();
//        for (auto& task : pair.second) {
//            tasks.push_back(task);
//        }
//        LOG(INFO) << "after splice tasks.size:" << tasks.size();
//    }
//}
void TaskCollection::takeSnapshot() {
}
void TaskCollection::loadSnapshot() {
}
void TaskCollection::apply(shared_ptr<Entry> entry) {
}
void TaskCollection::writeToStream(std::ostream& os) {
}
void TaskCollection::readFromStream(std::istream& is) {
}
}
