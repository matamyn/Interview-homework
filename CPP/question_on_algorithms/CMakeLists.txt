
set(TARGET_NAME alg)
set(SOURCE BST.cpp
        strstr_rating.cpp)

foreach (src ${SOURCE})
    add_executable(${TARGET_NAME}_${src} ${src})
endforeach ()