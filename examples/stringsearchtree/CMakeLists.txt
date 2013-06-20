set(sstt_sources
  stringsearchtreetest.cpp
)
include_directories(../../src)
add_executable(sstt ${sstt_sources})
target_link_libraries(sstt humbletreelib)
