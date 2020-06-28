add_subdirectory(moving_average)
add_subdirectory(sample_variance)

set(TARGET_NAME awg_test_task)

set(src main.cpp)
add_executable(${TARGET_NAME} ${src})
target_link_libraries(${TARGET_NAME} PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        common_cpp
        Qt5::Core
        moving_average
        sample_variance
        )