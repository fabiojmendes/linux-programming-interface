# ch3
file(GLOB source "*.c")
add_source(${source})

if(APPLE)
  set_target_properties(reboot PROPERTIES EXCLUDE_FROM_ALL 1)
endif()
