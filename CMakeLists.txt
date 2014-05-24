CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)
Project(BillsSplitter)

#find the Qt Libraries
find_package(Qt5Widgets)
include_directories(${Qt5Widget_INCLUDE_DIRS})

#find the Boost libraries
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(BillsSplitter)