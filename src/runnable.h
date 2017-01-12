#ifndef _CELESTIAL_RUNNABLE_H_
#define _CELESTIAL_RUNNABLE_H_

#include <thread>
#include <functional> 
#include <memory>

namespace celestial {
class Runnable {
public:
    virtual ~Runnable(){}
    void start() {
        mythread_ptr_ = std::unique_ptr<std::thread>(new std::thread(std::bind(&Runnable::run, this)));
    }
    void join() {
        if (mythread_ptr_.get()) {
            mythread_ptr_->join();
        }
    }
protected:
    virtual void run() = 0;
private:
    std::unique_ptr<std::thread> mythread_ptr_;
};
}

#endif
