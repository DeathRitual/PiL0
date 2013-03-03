cmake_minimum_required(VERSION 2.6)
project(PiL0)

include_directories("${PROJECT_SOURCE_DIR}/header")
add_subdirectory(header)
add_executable(PiL0 pl.c)
target_link_libraries(PiL0 header)

install(TARGETS PiL0 RUNTIME DESTINATION bin)
