set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.1)
  set(CMAKE_CXX_STANDARD 14)
ELSE ()
  add_compile_options(-std=c++11)
ENDIF ()

project( openBibleViewer )
cmake_minimum_required( VERSION 2.8.12 )
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )


MESSAGE( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )
MESSAGE( STATUS "CMAKE_INCLUDE_PATH: " ${CMAKE_INCLUDE_PATH} )

include(InstallRequiredSystemLibraries)

#### debug or release ####
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Using build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    option( OBV_NO_DEBUG_OUTPUT "Do not show Debug output" ON )
else()
    option( OBV_NO_DEBUG_OUTPUT "Do not show Debug output" OFF )
    option( OBV_DEBUG "Do not show Debug output" ON )

endif()

#### options ####
option( OBV_PORTABLE_VERSION "Build a portable version" OFF )
option( OBV_SWORD_SUPPORT "Build with sword support" OFF )
option( OBV_USE_QUAZIP "Use QuaZip" OFF)

#we have to use toStdWString() on linux
if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    option( OBV_USE_WSTRING "Use toStdWString and fromWCharArray" OFF)
else()
    option( OBV_USE_WSTRING "Use toStdWString and fromWCharArray" ON)
endif()

#### constants ####
set(OBV_VERSION_NUMBER "0.9.0")
set(OBV_VERSION_STATUS "stable")
set(OBV_BUILD_DATE "2017-11-21")

#### packages ####
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Script REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)


find_package( CLucene1 REQUIRED)

# optional quazip, we are using by default unzip or 7za.exe on windows
IF(OBV_USE_QUAZIP)
  find_package( QuaZip REQUIRED)
endif()

if(UNIX)
  include(FindZLIB)
endif()

if(APPLE)
  include(FindZLIB)
endif()

## Sword
if(OBV_SWORD_SUPPORT)
    include (FindSword)
    if (NOT SWORD_NOT_FOUND)
        ADD_DEFINITIONS(-DBUILD_WITH_SWORD)
    else()
        set(OBV_SWORD_SUPPORT FALSE)
    endif()
endif(OBV_SWORD_SUPPORT)

if(OBV_SWORD_SUPPORT)
  set(SW_LIBS ${CURL_LIBRARIES} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES} ${SWORD_LIBRARY})
  include_directories(
    ${SWORD_INCLUDE_DIR}
  )
else(OBV_SWORD_SUPPORT)
    set(SW_LIBS "")
endif(OBV_SWORD_SUPPORT)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Sql_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
  ${Qt5Network_INCLUDE_DIRS}
  ${Qt5Script_INCLUDE_DIRS}
  ${Qt5WebKitWidgets_INCLUDE_DIRS}
  ${Qt5WebEngineWidgets_INCLUDE_DIRS}
  ${Qt5PrintSupport_INCLUDE_DIRS}

  ${openBibleViewer_BINARY_DIR}/src # for config file
  ${CLUCENE1_INCLUDE_DIRS}
  ${MCLUCENE_BIN_SHARED}
  ${MCLUCENE_SRC_CORE}
  ${MCLUCENE_SRC_SHARED}
  ${QuaZip_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIRS}
)
#include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_subdirectory( src/extern )
add_subdirectory( src/core )

# sources
include(sources)

if(APPLE)
  include(osx)
elseif(WIN32)
  include(windows)
else()
  include(linux)
endif()
