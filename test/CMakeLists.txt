add_executable(yulskers_main main.cpp)
target_link_libraries(yulskers_main yulskers)
target_compile_options(yulskers_main PUBLIC -ftemplate-depth=2000)
