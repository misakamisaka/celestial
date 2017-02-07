#ifndef _CELESTIAL_TASK_COLLECTION_H_
#define _CELESTIAL_TASK_COLLECTION_H_

#include <unordered_map>
#include <string>
#include <list>
#include <memory>
#include <vector>
#include "task_info_types.h"
#include "check_point_types.h"

namespace task_manager {

typedef std::shared_ptr<TaskInfo> TaskPtr;

class TaskCollection : public StateMachine, public std::enable_shared_from_this<TaskCollection> {
public:
    //read from the log and apply entry
    virtual void run() override;

    virtual std::shared_ptr<Snapshot> takeSnapshot() override;
    virtual void loadSnapshot(std::shared_ptr<Snapshot>) override;
    virtual void apply(const std::vector<shared_ptr<Entry> >& entries) override;
    virtual void writeToStream(std::ostream& os) override;
    virtual void readFromStream(std::istream& is) override;

    void getTasksInGroup(const std::string& group_name, std::vector<TaskPtr>& tasks);
    //bool update(std::vector<int64_t>& delete_ids, std::vector<TaskPtr>& insert_tasks, const std::string& worker_name);
    void getAll(std::list<TaskPtr>& tasks);
private:
    std::unordered_map<int64_t, std::list<TaskPtr>::iterator> tasks_map_;
    std::unordered_map<std::string, std::list<TaskPtr> > groups_map_;

    //state machine read entry from log and advance appliedindex
    std::shared_ptr<Log> log_;
    std::shared_ptr<RaftContext> context_;
    int64_t last_applied_index_;
    int64_t last_applied_term_;
    int64_t max_task_id_;
};
}

#endif //_CELESTIAL_TASK_COLLECTION_H_
