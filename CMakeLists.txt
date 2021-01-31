cmake_minimum_required(VERSION 3.10)
project(commandrunner)




#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

set(CMAKE_C_FLAGS "-g -Wall -Werror -O3 -std=c99 -pie -fPIC")

# make commandrunner lib
file(GLOB SOURCE "src/*.c" "src/rules/*.c")
add_executable(libcommandrunner.so ${SOURCE})
target_link_libraries(libcommandrunner.so pthread seccomp)


install(FILES output/libcommandrunner.so
    PERMISSIONS OWNER_EXECUTE OWNER_READ
    DESTINATION /usr/lib/commandrunner)
