project( openBibleViewer )
cmake_minimum_required( VERSION 2.6 )

set ( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
add_definitions ( -Wall -g)
find_package ( Qt4 REQUIRED )
include (FindThreads)
include (FindZlib)

if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif (NOT CMAKE_BUILD_TYPE)

message("-- Using build type: ${CMAKE_BUILD_TYPE}")

include_directories (
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR} 
    ${QT_QTXML_INCLUDE_DIR} 
    ${QT_QTNETWORK_INCLUDE_DIR} 
    ${QT_QTWEBKIT_INCLUDE_DIR} 
    ${CLucene_INCLUDEDIR} 
    ${openBibleViewer_BINARY_DIR}/src
    ${openBibleViewer_BINARY_DIR}/src/clucene/shared
    ${openBibleViewer_SOURCE_DIR}/src/clucene/shared
    ${openBibleViewer_SOURCE_DIR}/src/clucene/core
    )

add_subdirectory ( src )
add_subdirectory ( src/clucene/core )
add_subdirectory ( src/clucene/shared )