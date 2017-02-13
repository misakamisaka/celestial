cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(raft raft_context.cpp election_timeout_checker.cpp raft_manager.cpp metadata.cpp raft_peer.cpp)
TARGET_LINK_LIBRARIES(raft glog libboost_system.a libboost_filesystem.a libjsoncpp.a)
