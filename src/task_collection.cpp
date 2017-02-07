#include "task_collection.h"
#include <glog/logging.h>
#include <boost/shared_ptr.hpp>

using std::list;
using std::string;
using std::Vector;
using std::shared_ptr;

namespace task_manager {

void TaskCollection::run() {
    void getEntryFromIndex(int64_t index, uint32_t& number_of_entry, 
        std::vector<std::shared_ptr<Entry> >& entries_result);
    std::vector<shared_ptr<Entry> > entries;
    log_->getEntryFromIndex(last_applied_index_ + 1, context_->commit_index - last_applied_index_, entries);
    apply(entries);
    
}

std::shared_ptr<Snapshot> TaskCollection::takeSnapshot() {
    std::shared_ptr<Snapshot> snapshot = std::make_shared<Snapshot>();
    snapshot->serializeSnapshot(shared_from_this());
    snapshot->setLastIncludedIndex(last_applied_index_);
    snapshot->setLastIncludedTerm(last_applied_term_);
    return snapshot;
}

void TaskCollection::loadSnapshot(std::shared_ptr<Snapshot> snapshot) {
    last_applied_index_ = snapshot->getLastIncludedIndex();
    last_applied_term_ = snapshot->getLastIncludedTerm();
    std::shared_ptr<StateMachine> state_machine = snapshot->deserializeSnapshot();
    tasks_map_ = state_machine->tasks_map_;
    groups_map_ = state_machine->groups_map_;
}

void TaskCollection::apply(const vector<shared_ptr<Entry> >& entries) {
    for (auto& entry: entries) {
        last_applied_index_ ++;
        last_applied_term_ = entry.term;
        if (entry->entry_type != EntryType::DATA) {
            continue;
        }
        boost::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
        boost::shared_ptr<TBinaryProtocol> binaryProtcol(new TBinaryProtocol(buffer));
        buffer->resetBuffer((uint8_t*)entry->data.data(), entry->data.size());
        UpdateRequest update_request;
        update_request.read(binaryProtcol.get());
        update_request.from(tmp_checkpoint);

        for (auto& del_task : update_request.deletions) {
            auto it = tasks_map_.find(del_task.task_id);
            if (it == tasks_map_.end()) {
                LOG(INFO) << "del_task.task_id[" << del_task.task_id << "] not exist";
                goto next_loop;
            } else if (!worker_name.empty() &&
                    !(*(it->second))->owner.empty() &&
                    (*(it->second))->owner != worker_name) {
                LOG(INFO) << "id[" << del_task.task_id << "] owner:" << (*(it->second))->owner << ", worker_name:" << worker_name;
                goto next_loop;
            }
        }

        for (auto& task : inserts) {
            list<TaskPtr>& task_list = groups_map_[task->group];
            auto it = task_list.insert(task_list.end(), std::make_shared<TaskInfo>(task));
            (*it)->__set_task_id(++max_task_id_);
            tasks_map_.insert(std::pair<int64_t, list<TaskPtr>::iterator>(task->task_id, it)); 
        }

        for (auto& del_task : update_request.deletions) {
            auto it = tasks_map_[del_task.task_id];
            groups_map_[(*it)->group].erase(it);
        }
        next_loop:
    }
}

void TaskCollection::writeToStream(std::ostream& os) {
    SnapshotInfo snapshot_info;
    for (auto& it : tasks_map_) {
        snapshot_info.tasks.push_back(*(it.second));
    }
    boost::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
    boost::shared_ptr<TBinaryProtocol> binaryProtcol(new TBinaryProtocol(buffer));
    snapshot_info.write(binaryProtcol.get());
    string snapshot_info_str = buffer->getBufferAsString();
    os.write(snapshot_info_str.data(), snapshot_info_str.size());
}

void TaskCollection::readFromStream(std::istream& is) {
    string snapshot_info_str(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
    boost::shared_ptr<TMemoryBuffer> buffer(new TMemoryBuffer());
    boost::shared_ptr<TBinaryProtocol> binaryProtcol(new TBinaryProtocol(buffer));
    buffer->resetBuffer((uint8_t*)snapshot_info_str.data(), snapshot_info_str.length());
    SnapshotInfo snapshot_info;
    snapshot_info.read(binaryProtcol.get());
    for (auto& task : snapshot_info.tasks) {
        list<TaskPtr>& task_list = groups_map_[task.group];
        auto it = task_list.insert(task_list.end(), std::make_shared<TaskInfo>(task));
        (*it)->__set_task_id(++max_task_id_);
        tasks_map_.insert(std::pair<int64_t, list<TaskPtr>::iterator>(task->task_id, it));
    }
}

void TaskCollection::getTasksInGroup(const std::string& group_name, std::vector<TaskPtr>& tasks) {
    std::list<TaskPtr>& tasks_list = groups_map_[group_name];
    tasks.assign(tasks_list.begin(), tasks_list.end());
}

void TaskCollection::getAll(std::list<TaskPtr>& tasks) {
    for (auto& pair : groups_map_) {
        for (auto& task : pair.second) {
            tasks.push_back(task);
        }
    }
}
}
