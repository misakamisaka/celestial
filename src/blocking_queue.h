#ifndef  _CELESTIAL_BLOCKING_QUEUE_H_
#define  _CELESTIAL_BLOCKING_QUEUE_H_

#include <deque>
#include <mutex>
#include <condition_variable>

namespace task_manager {
template<class T>
class BlockingQueue {
public:
    BlockingQueue() : _max_size(100000) { }
    BlockingQueue(int max_size) : _max_size(max_size) { }

    void put(const T& x);
    void take(T& x);
    bool offer(T& x, int wait_microsecs);
    bool poll(T& x, int wait_microsecs);

    size_t size();
    bool full();
    bool empty();
    void clear();

private:
    BlockingQueue(const BlockingQueue&) = delete;
    BlockingQueue& operator= (const BlockingQueue&) = delete;
    mutable std::mutex _mutex;
    std::condition_variable _not_empty;
    std::condition_variable _not_full;
    size_t _max_size;
    std::deque<T> _queue;
};
}
#include "blocking_queue.hpp"

#endif  //BLOCKING_QUEUE_H
