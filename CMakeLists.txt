cmake_minimum_required(VERSION 2.8.12)
project (Timbus)



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
add_subdirectory(cgraph)

add_executable(main main-20194562.c)
target_link_libraries(main cgraph)

add_subdirectory(libfdr)
target_link_libraries(main fdr)