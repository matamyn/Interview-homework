set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (TARGET_NAME moving_average)

set (src MovingAverage.cpp)
add_library(${TARGET_NAME} STATIC ${src})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})