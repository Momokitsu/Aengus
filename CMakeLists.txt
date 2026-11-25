# 指定版本要求
cmake_minimum_required(VERSION 3.25)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE FILEPATH "C compiler" FORCE)
    set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE FILEPATH "C++ compiler" FORCE)
endif()

# 指定编译器
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# 定义项目
project(aengus VERSION 0.1.0 LANGUAGES CXX C)

# 配套分发支持
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 编译开关
option(AENGUS_BUILD_EXAMPLES "Build examples" ON)
option(AENGUS_BUILD_TESTS    "Build tests"    ON)
option(AENGUS_BUILD_TOOLS    "Build tools"    ON)

# 编译模式
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# 设置并行编译线程数
cmake_host_system_information(RESULT CPU_NUMS QUERY NUMBER_OF_LOGICAL_CORES)
math(EXPR JOBS "${CPU_NUMS} + ${CPU_NUMS} / 2")
set(CMAKE_BUILD_PARALLEL_LEVEL ${JOBS} CACHE STRING "并行编译线程数")
message(STATUS "Detected ${CPU_NUMS} logical cores, suggested: cmake --build ${CMAKE_BINARY_DIR} --parallel ${JOBS}")



include(cmake/aengusOptions.cmake)



add_subdirectory(aengus)
if(AENGUS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if(AENGUS_BUILD_TOOLS)
    add_subdirectory(tools)
endif()