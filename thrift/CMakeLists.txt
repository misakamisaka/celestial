cmake_minimum_required(VERSION 2.8)

#include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/thrift)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/)
AUX_SOURCE_DIRECTORY(. SRC)
ADD_EXECUTABLE(unittest ${SRC})
TARGET_LINK_LIBRARIES(unittest tm tm_thrift gtest thrift jsoncpp boost_filesystem boost_system glog boost_timer pthread)
