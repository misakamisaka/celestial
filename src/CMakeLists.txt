cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(celetial_service configure.cpp)
TARGET_LINK_LIBRARIES(celetial_service libjsoncpp.a)
