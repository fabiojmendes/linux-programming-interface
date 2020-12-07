cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

project(hello-linux)

add_compile_definitions(DEBUG)
add_compile_options(-ggdb3 -Og -Wall -Wextra)

# Build 32 bits
add_compile_options(-m32)
add_link_options(-m32)

function(add_source)
    foreach(file_path IN LISTS ARGV)
        string(REPLACE ".c" "" new_name ${file_path} )
        get_filename_component(filename ${new_name} NAME)
        add_executable(${filename} ${file_path} )
    endforeach()
endfunction()

# Add all submodules
file(GLOB modules "ch*")
foreach(mod ${modules})
    add_subdirectory(${mod})
endforeach()
