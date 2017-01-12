#include "blocking_queue.h"
#include <boost/timer/timer.hpp>

namespace task_manager {
template<class T>
void BlockingQueue<T>::put(const T& x) {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_queue.size() >= _max_size) {
        _not_full.wait(lock);
    }
    _queue.push_back(x);
    _not_empty.notify_one(); 
}

template<class T>
void BlockingQueue<T>::take(T& x) {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_queue.empty()) {
        _not_empty.wait(lock);
    }
    x = _queue.front();
    _queue.pop_front();
    _not_full.notify_one();
}

template<class T>
bool BlockingQueue<T>::offer(T& x, int wait_microsecs) {
    boost::timer::cpu_timer my_timer;
    std::unique_lock<std::mutex> lock(_mutex);
    for(;;) {
        if(!full()) {
            _queue.push_back(x);
            _not_empty.notify_one(); 
            return true;
        } else {
            _not_empty.wait_for(lock, std::chrono::nanoseconds(wait_microsecs * 1000));
        }

        if(my_timer.elapsed().wall >= wait_microsecs * 1000) {
            return false;
        }
    }
    return false;
}

template<class T>
bool BlockingQueue<T>::poll(T& x, int wait_microsecs) {
    boost::timer::cpu_timer my_timer;
    std::unique_lock<std::mutex> lock(_mutex);
    for(;;) {
        if(!_queue.empty()) {
            x = _queue.front();
            _queue.pop_front();
            _not_full.notify_one();
            return true;
        } else {
            _not_empty.wait_for(lock, std::chrono::nanoseconds(wait_microsecs * 1000));
        }

        if(my_timer.elapsed().wall >= wait_microsecs * 1000) {
            return false;
        }
    }
    return false;
}

template<class T>
size_t BlockingQueue<T>::size() {
    return _queue.size();
}

template<class T>
bool BlockingQueue<T>::full() {
    return _queue.size() >= _max_size;
}

template<class T>
bool BlockingQueue<T>::empty() {
    return _queue.empty();
}

template<class T>
void BlockingQueue<T>::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    while (!_queue.empty()) {
        _queue.pop_front(); 
    }
}
}
