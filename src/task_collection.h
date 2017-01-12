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

class TaskCollection : public StateMachine {
public:
    virtual std::shared_ptr<StateMachine> takeSnapshot() override;
    virtual void loadSnapshot(std::shared_ptr<StateMachine>) override;
    virtual void apply(shared_ptr<Entry> entry) override;
    virtual void writeToStream(std::ostream& os) override;
    virtual void readFromStream(std::istream& is) override;
    //TaskCollection(Checkpoint& checkpoint);
    //void getTasksInGroup(const std::string& group_name, std::vector<TaskPtr>& tasks);
    //bool update(std::vector<int64_t>& delete_ids, std::vector<TaskPtr>& insert_tasks, const std::string& worker_name);
    //void getAll(std::list<TaskPtr>& tasks);
    //read from the log and apply entry
    void run();
protected:
    virtual void writeToStream(std::ostream& os) override;
    virtual void readFromStream(std::istream& is) override;
    
private:
    std::unordered_map<int64_t, std::list<TaskPtr>::iterator> tasks_map_;
    std::unordered_map<std::string, std::list<TaskPtr> > groups_map_;

    //state machine read entry from log and advance appliedindex
    std::shared_ptr<Log> log_;
    std::shared_ptr<Snapshot> snapshot_;
    int64_t lasf_applied_index_;
};
}

#endif //_CELESTIAL_TASK_COLLECTION_H_
