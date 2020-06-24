set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (TARGET_NAME sample_variance)

set (src SampleVariance.cpp SampleVariance.h)
add_library(${TARGET_NAME} STATIC ${src})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_INCLUDE_CURRENT_DIR})