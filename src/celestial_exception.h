#ifndef CELESTIAL_EXCEPTION_H
#define CELESTIAL_EXCEPTION_H

#include <exception>
#include <string>

namespace celestial {

class CelestialException : public std::exception {
public:
    CelestialException(const std::string& what_str)
        :err_msg_(what_str) {}
    virtual ~CelestialException() throw() {}
    virtual const char* what() {
        return err_msg_.c_str();
    }
protected:
    std::string err_msg_;
};
}

#endif //CELESTIAL_EXCEPTION_H
