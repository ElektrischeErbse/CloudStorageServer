include(FetchContent)

fetchcontent_declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json
)
fetchcontent_makeavailable(nlohmann_json)

# fetchcontent_declare(
#     cpp-httplib
#     GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
#     GIT_TAG v0.15.3
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib
# )
# fetchcontent_makeavailable(cpp-httplib)

fetchcontent_declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spdlog
)
fetchcontent_makeavailable(spdlog)

fetchcontent_declare(
    ormpp
    GIT_REPOSITORY https://github.com/qicosmos/ormpp.git
    GIT_TAG 0.2.1
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ormpp
)
fetchcontent_makeavailable(ormpp)

fetchcontent_declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-38-0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/asio
)
fetchcontent_makeavailable(asio)

fetchcontent_declare(
    googletest
    # URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest
)
fetchcontent_makeavailable(googletest)
