set(PROJECT_NAME App)

set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2-2.0.5")
set(SDL2_INCLUDE_DIR "${SDL2_PATH}/include")
set(SDL2_LIBRARY "${SDL2_PATH}/lib/x86/SDL2.lib" "${SDL2_PATH}/lib/x86/SDL2main.lib")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src" ${SDL2_INCLUDE_DIR})

set(Type WIN32)
add_executable(${PROJECT_NAME} ${Type} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} LifeCicle ${SDL2_LIBRARY})
