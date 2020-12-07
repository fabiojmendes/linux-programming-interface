# ch5
file(GLOB source "*.c")
add_source(${source})

target_compile_definitions(large_files PRIVATE _FILE_OFFSET_BITS=64)
