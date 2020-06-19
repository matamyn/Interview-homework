add_subdirectory(moving_average)
add_subdirectory(sample_variance)

set (TARGET_NAME awg_test_task)

set (src main.cpp)

add_executable(${TARGET_NAME} ${src})