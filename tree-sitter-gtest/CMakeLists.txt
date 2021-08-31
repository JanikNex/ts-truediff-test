cmake_minimum_required(VERSION 3.19)
project(tree-sitter-gtest)

add_subdirectory(lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

############ Exp ###############
add_executable(GTest-Exp libs/tree-sitter-exp/parser.c libs/tree-sitter-exp/tree_sitter/parser.h SimpleExpTest.cpp IncrementalExpTest.cpp)

target_link_libraries(GTest-Exp ${CMAKE_SOURCE_DIR}/tree-sitter-gtest/libs/tree-sitter/lib/libtree-sitter.a)
target_link_libraries(GTest-Exp m)

target_link_libraries(GTest-Exp gtest gtest_main)


############ PQueue #############
add_executable(GTest-PQueue PQueueTest.cpp libs/pqueue/pqueue.c libs/pqueue/pqueue.h)

target_link_libraries(GTest-PQueue ${CMAKE_SOURCE_DIR}/tree-sitter-gtest/libs/tree-sitter/lib/libtree-sitter.a)
target_link_libraries(GTest-PQueue m)

target_link_libraries(GTest-PQueue gtest gtest_main)


############ Java ###############
add_executable(GTest-Java libs/tree-sitter-java/parser.c libs/tree-sitter-java/tree_sitter/parser.h SimpleJavaTest.cpp)

target_link_libraries(GTest-Java ${CMAKE_SOURCE_DIR}/tree-sitter-gtest/libs/tree-sitter/lib/libtree-sitter.a)
target_link_libraries(GTest-Java m)

target_link_libraries(GTest-Java gtest gtest_main)

