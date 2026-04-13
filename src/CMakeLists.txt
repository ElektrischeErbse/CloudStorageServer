set(MYSQL_INCLUDE_DIR "/usr/include/mysql")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
find_library(LIB_MYSQL mysqlclient)

add_executable(main main.cpp
    tcp_server.cpp
    tcp_session.cpp
    logger.cpp
    protocol.cpp
    db_singleton.cpp
    user_dao.cpp
    user_service.cpp
    json_handler.cpp
)
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    # ormpp include path
    ${CMAKE_SOURCE_DIR}/third_party/ormpp
    # mysql include path
    ${MYSQL_INCLUDE_DIR}
    # asio include path
    ${CMAKE_SOURCE_DIR}/third_party/asio/include
)
target_link_libraries(main PRIVATE
    ${LIB_MYSQL}
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)