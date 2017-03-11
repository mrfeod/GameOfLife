cmake_minimum_required (VERSION 3.5)
project (Hello)

set(SRC_PATH "${CMAKE_CURRENT_LIST_DIR}/src")

include(${SRC_PATH}/Life/LifeCicle.cmake)


set(HELLO_SOURCE_DIR ${SRC_PATH}/App)

include_directories(${SRC_PATH})

add_executable(Hello ${HELLO_SOURCE_DIR}/main.cpp)
target_link_libraries(Hello LifeCicle)
