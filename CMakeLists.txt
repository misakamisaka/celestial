cmake_minimum_required(VERSION 2.8)
project(celestial)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -O2 -fPIC")
set(WORKSPACE ${PROJECT_SOURCE_DIR}/../)
include_directories(./include ./src ./thrift ./log ./raft ${WORKSPACE}/third/jsoncpp/include)
link_directories(${PROJECT_SOURCE_DIR}/lib/ ${WORKSPACE}/third/jsoncpp/lib)

add_subdirectory(log)
add_subdirectory(src)
add_subdirectory(raft)
add_subdirectory(thrift)
add_subdirectory(test)
