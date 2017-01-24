#ifndef  RAFT_EXCEPTION_H
#define  RAFT_EXCEPTION_H

#include <exception>
#include <string>

namespace celestial {

class RaftException : public std::exception {
public:
    RaftException(const std::string& what_str)
        :err_msg_(what_str) {}
    virtual ~RaftException() throw() {}
    virtual const char* what() {
        return err_msg_.c_str();
    }
protected:
    std::string err_msg_;
};
}

#endif  //RAFT_EXCEPTION_H
