# ch52
if(NOT APPLE)
  file(GLOB source "*.c")
  add_source(${source})

  target_link_libraries(posix_queue_pub rt)
  target_link_libraries(posix_queue_sub rt)
endif()
