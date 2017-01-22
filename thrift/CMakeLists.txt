cmake_minimum_required(VERSION 2.8)

include_directories(${WORKSPACE}/third/thrift/include)
link_directories(${WORKSPACE}/third/thrift/lib)

AUX_SOURCE_DIRECTORY(. THRIFT_SRC)
ADD_LIBRARY(celestial_thrift ${THRIFT_SRC})
TARGET_LINK_LIBRARIES(celestial_thrift libthrift.a)
