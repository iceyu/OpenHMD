project (simple C)

set(${CMAKE_BUILD_TYPE} Debug)

include_directories(${CMAKE_BINARY_DIR}/ ${CMAKE_CURRENT_LIST_DIR}/../../src/drv_wick)
link_directories(${CMAKE_BINARY_DIR})
add_executable(simple simple.c)
target_link_libraries(simple PRIVATE openhmd)

if (UNIX)
    target_link_libraries(simple PRIVATE m)
endif()
