cmake_minimum_required(VERSION 2.8)
project(humble_tree_library)

set(sources
  src/ternarytree/stringsearchtree.cpp
)

if(WIN32)
  add_definitions(/Zc:wchar_t-)
endif(WIN32)

include_directories(src)

add_library(humbletreelib STATIC ${sources})

add_subdirectory(examples)
