cmake_minimum_required(VERSION 3.11)

project(OnnxEdit VERSION 0.0.0 LANGUAGES CXX)

cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(ENABLE_TEST "If enable the test" ON)
option(BUILD_SHARED_LIBS "If build as static mode" OFF)
option(QT_PREFIX_PATH "QT LIB PATH" "${CMAKE_SOURCE_DIR}/3rd_party/qtbase-release")

set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(3rd_party)

set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH} ${QT_PREFIX_PATH})
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(utils)
add_subdirectory(gui)
if(ENABLE_TEST)
    enable_testing()
    add_subdirectory(unit_test)
endif()

add_executable(main
    main.cpp
)

target_link_libraries(
    main
    PUBLIC
    gflags::gflags
    glog::glog
    
    gui
)
