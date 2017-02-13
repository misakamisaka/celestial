cmake_minimum_required(VERSION 2.8)

ADD_EXECUTABLE(log_file_test test_log_file.cpp)
TARGET_LINK_LIBRARIES(log_file_test gtest celetial_log celetial_service)

ADD_EXECUTABLE(log_test test_log.cpp)
TARGET_LINK_LIBRARIES(log_test gtest celetial_log celetial_service)

ADD_EXECUTABLE(test_raft_context test_raft_context.cpp)
TARGET_LINK_LIBRARIES(test_raft_context gtest celetial_log celetial_service raft)
