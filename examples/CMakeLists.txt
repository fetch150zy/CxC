# 示例程序

add_executable(basic_usage basic_usage.c)
target_link_libraries(basic_usage PRIVATE cxc_static)
target_include_directories(basic_usage PRIVATE ${CMAKE_SOURCE_DIR}/src)
