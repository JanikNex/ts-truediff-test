cmake_minimum_required(VERSION 3.19)

project(tree_sitter_test)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("${PROJECT_SOURCE_DIR}/tree-sitter-bench")
add_subdirectory("${PROJECT_SOURCE_DIR}/tree-sitter-bench-inc2")
add_subdirectory("${PROJECT_SOURCE_DIR}/tree-sitter-gtest")