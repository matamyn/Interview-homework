set(TARGET_NAME common_cpp)
set(src RingBuffer.cpp )

add_library(${TARGET_NAME} STATIC ${src})
target_link_libraries(${TARGET_NAME} PUBLIC Qt5::Core)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})