cmake_minimum_required(VERSION 3.16)

project(WeatherProject LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ---------- 基本设置 ----------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 开启 Qt 的自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 让 uic 在 ui/ 目录中查找 .ui 文件
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# ---------- 查找 Qt ----------
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network Core Widgets REQUIRED)

# ---------- 获取 nlohmann_json ----------
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# ---------- 获取 jwt-cpp ----------

fetchcontent_declare(jwt-cpp 
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG 08bcf77a687fb06e34138e9e9fa12a4ecbe12332 # v0.7.0 release
)
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "disable building examples" FORCE)
fetchcontent_makeavailable(jwt-cpp)




# ---------- 收集源文件 ----------
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.c
)
add_library(core STATIC ${CORE_SOURCES})

# 手动 moc entity.hpp，确保 moc 输出被编译进 core
qt6_wrap_cpp(CORE_MOC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/core/entity.hpp
)
target_sources(core PRIVATE ${CORE_MOC_SRCS})

# 关掉 core 的 AUTOMOC，避免重复生成
set_target_properties(core PROPERTIES AUTOMOC OFF)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/core
)
file(GLOB_RECURSE UTILS_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.c
)
add_library(utils STATIC ${UTILS_SOURCES})
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.hpp
)

file(GLOB_RECURSE PROJECT_UI CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.ui
)
set_target_properties(core PROPERTIES AUTOMOC ON)
# ---------- 可执行文件 ----------
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UI}
)

# ---------- 包含目录 ----------
target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/core
)
target_link_libraries(core PUBLIC
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    utils
)

target_include_directories(utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/utils
)
target_link_libraries(utils PUBLIC
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------- 链接库 ----------
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
    core
    utils
)
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://gitee.com/mirrors/googletest.git
        GIT_TAG        v1.14.0
    )
    # Windows 下避免覆盖父项目的编译选项
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # 关闭 GoogleTest 自带的 install 规则，避免污染安装
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    add_subdirectory(tests)
endif()
