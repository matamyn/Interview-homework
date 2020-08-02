set(TARGET_NAME cod)
set(SOURCE cod.cpp)

foreach (src ${SOURCE})
    add_executable(${TARGET_NAME} ${src})
endforeach ()