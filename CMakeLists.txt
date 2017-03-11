cmake_minimum_required (VERSION 3.5)
project (App)

set(SRC_PATH "${CMAKE_CURRENT_LIST_DIR}/src")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")

include(${SRC_PATH}/Life/LifeCicle.cmake)
include(${SRC_PATH}/App/App.cmake)

install (TARGETS App DESTINATION ${BIN_DIR})
