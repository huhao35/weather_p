file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(unit_tests ${TEST_SOURCES})

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
    core
    utils
)
# 如果需要测试 Qt 相关代码，再链接 Qt
# target_link_libraries(unit_tests PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GoogleTest)
gtest_discover_tests(unit_tests)
