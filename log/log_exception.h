#ifndef  LOG_EXCEPTION_H
#define  LOG_EXCEPTION_H

#include "raft_exception.h"
#include <string>

namespace celestial {

class LogException : public RaftException {
public:
    LogException(const std::string& what_str)
        : RaftException(what_str) {}
    virtual ~LogException() throw() {}
    virtual const char* what() {
        return err_msg_.c_str();
    }
};
}

#endif  //LOG_EXCEPTION_H
