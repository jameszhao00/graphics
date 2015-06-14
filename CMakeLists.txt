cmake_minimum_required(VERSION 3.2)
project(graphics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp mesh.capnp.c++)
add_executable(graphics ${SOURCE_FILES})

include_directories(/Users/jameszhao/assimp/include)
target_link_libraries(graphics capnp kj assimp glog)