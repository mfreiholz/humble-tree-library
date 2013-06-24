set(rtt_sources
  radixtreetest.cpp
)
include_directories(../../src)
add_executable(rtt ${rtt_sources})
target_link_libraries(rtt humbletreelib)
