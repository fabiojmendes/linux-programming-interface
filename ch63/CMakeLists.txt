# ch63
file(GLOB source "*.c")
add_source(${source})

if(APPLE)
  set_target_properties(epoll PROPERTIES EXCLUDE_FROM_ALL 1)
endif()
