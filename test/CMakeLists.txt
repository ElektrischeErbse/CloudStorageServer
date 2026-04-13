add_executable(test_json_handler
    test_json_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/json_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/logger.cpp
)
target_include_directories(test_json_handler PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_json_handler PRIVATE
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    GTest::gtest
)

add_executable(test_protocol
    test_protocol.cpp
    ${CMAKE_SOURCE_DIR}/src/protocol.cpp
    ${CMAKE_SOURCE_DIR}/src/logger.cpp
)
target_include_directories(test_protocol PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_protocol PRIVATE
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    GTest::gtest
)

set(MYSQL_INCLUDE_DIR "/usr/include/mysql")
find_library(LIB_MYSQL mysqlclient)
add_executable(test_user_dao
    test_user_dao.cpp
    ${CMAKE_SOURCE_DIR}/src/user_dao.cpp
    ${CMAKE_SOURCE_DIR}/src/db_singleton.cpp
)
target_compile_definitions(test_user_dao PRIVATE -DUSER_DAO_TEST)
target_include_directories(test_user_dao PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/ormpp
    ${MYSQL_INCLUDE_DIR}
)
target_link_libraries(test_user_dao PRIVATE
    GTest::gtest
    ${LIB_MYSQL}
)

include(GoogleTest)
gtest_discover_tests(test_json_handler)
gtest_discover_tests(test_protocol)
gtest_discover_tests(test_user_dao)
