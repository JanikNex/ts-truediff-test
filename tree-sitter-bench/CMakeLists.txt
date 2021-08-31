cmake_minimum_required(VERSION 3.19)
project(bench)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_executable(bench libs/tree-sitter-python/scanner.cc libs/tree-sitter-python/parser.c bench.c)
#target_link_libraries(bench ${CMAKE_SOURCE_DIR}/tree-sitter-bench/libs/tree-sitter/lib/libtree-sitter.a)
target_link_libraries(bench ${CMAKE_SOURCE_DIR}/tree-sitter-bench/libs/tree-sitter/lib/tree-sitter.lib)
target_link_libraries(bench m)

target_link_libraries(bench "-static")