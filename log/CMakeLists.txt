cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(celetial_log log_file.cpp log.cpp)
TARGET_LINK_LIBRARIES(celetial_log glog celestial_thrift libboost_system.a
libboost_filesystem.a libjsoncpp.a)
