cmake_minimum_required(VERSION 3.2)
project(graphics)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(THIRD_PARTY_LIBRARIES capnp kj assimp glog)

set(SOURCE_FILES main.cpp mesh.capnp.c++)
add_executable(graphics ${SOURCE_FILES})

include_directories(/Users/jameszhao/assimp/include /Users/jameszhao/Downloads/gtest-1.7.0/include)
target_link_libraries(graphics ${THIRD_PARTY_LIBRARIES})



# Testing
add_subdirectory(thirdparty/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(runUnitTests test.h test.cpp)
target_link_libraries(runUnitTests gtest gtest_main)
target_link_libraries(runUnitTests ${THIRD_PARTY_LIBRARIES})