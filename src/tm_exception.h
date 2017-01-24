#ifndef  TM_EXCEPTION_H
#define  TM_EXCEPTION_H

#include <exception>
#include <string>

namespace task_manager {

class TMException : public std::exception {
public:
    TMException(const std::string& what_str)
        :err_msg_(what_str) {}
    virtual ~TMException() throw() {}
    virtual const char* what() {
        return err_msg_.c_str();
    }
protected:
    std::string err_msg_;
};
}

#endif  //TM_EXCEPTION_H
