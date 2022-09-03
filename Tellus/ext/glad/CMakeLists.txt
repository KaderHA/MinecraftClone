set(SRCS "src/glad.c" "include/glad/glad.h" "include/KHR/khrplatform.h")
add_library(glad STATIC ${SRCS})

target_include_directories(glad PUBLIC "include")