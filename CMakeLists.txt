cmake_minimum_required(VERSION 3.22)

project(cloud-storage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# enable ormpp mysql support
set(ENABLE_MYSQL ON)
add_definitions(-DORMPP_ENABLE_MYSQL)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
enable_testing()

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
