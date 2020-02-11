add_executable(yulskers_static_compile_time_tests static_compile_time_tests.cpp)
target_link_libraries(yulskers_static_compile_time_tests yulskers)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/catch2/cmake)

include(Catch)

add_executable(yulskers_test main.cpp)
target_link_libraries(yulskers_test yulskers)
target_include_directories(yulskers_test PUBLIC ${CMAKE_CURRENT_LIST_DIR})
catch_discover_tests(yulskers_test)