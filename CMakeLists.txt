project( openBibleViewer )
cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
add_definitions ( -Wall -g)
find_package ( Qt4 REQUIRED )
include (FindThreads)
include (FindZlib)

#build own clucene because no distribution ships clucene >= 0.9.23 
set( BUILD_STATIC_LIBRARIES "On")
set(CLUCENE_VERSION_MAJOR "0")
set(CLUCENE_VERSION_MINOR "9")
set(CLUCENE_VERSION_REVISION "23")
set(CLUCENE_VERSION_PATCH "0")
set(CLUCENE_INT_VERSION 92300)

set(CLUCENE_VERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}.${CLUCENE_VERSION_PATCH}")
set(CLUCENE_SOVERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}")



if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif (NOT CMAKE_BUILD_TYPE)
message("-- Using build type: ${CMAKE_BUILD_TYPE}")


option(ENABLE_DEBUG
"enable debug support"
ON)
option(ENABLE_MMAP
"enable mmap support (experimental)"
OFF)
option(DISABLE_MULTITHREADING
"disable multithreading - remove all locking code"
OFF)
option(ENABLE_DMALLOC
"enable dmalloc memory leak checker"
OFF)
option(ENABLE_ASCII_MODE
"enable ascii support"
OFF)
option(LUCENE_USE_INTERNAL_CHAR_FUNCTIONS
"use internal character functions. required to run tests correctly"
ON)
option(ENABLE_PACKAGING
"create build scripts for creating clucene packages"
OFF)
option(
"create targets for building static libraries"
OFF)
option(BUILD_CONTRIBS
"create targets for building the clucene-contribs"
OFF)
option(BUILD_CONTRIBS_LIB
"create targets for building the clucene-contribs-lib"
OFF)
SET(LUCENE_SYS_INCLUDES "" CACHE PATH
    "location for non-system independent files. defaults to CMAKE_INSTALL_PREFIX. see INSTALL documentation for further information."
    )


set ( ENABLE_COMPILE_TESTS_VALUE ON )
if ( MSVC_IDE )
    #this is annoying...
    set ( ENABLE_COMPILE_TESTS_VALUE OFF )
endif( MSVC_IDE )

option(ENABLE_COMPILE_TESTS "enable various projects that test alternative build switches" ${ENABLE_COMPILE_TESTS_VALUE})
option( _PORTABLE_VERSION "Build a portable version" OFF )

#check flags...
include (TestCXXAcceptsFlag)
if ( CMAKE_COMPILER_IS_GNUCC )
    CHECK_CXX_ACCEPTS_FLAG(-pg GccFlagPg)
    if ( GccFlagPg )
        option(ENABLE_GPROF
        "turn on gprof profiling support"
        OFF)

        if ( ENABLE_GPROF )
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
            SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -pg")
        endif ( ENABLE_GPROF )
    endif ( GccFlagPg )
    
    if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
    endif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")

endif(CMAKE_COMPILER_IS_GNUCC) 

include_directories ( ${openBibleViewer_SOURCE_DIR}/src/clucene/core )
include_directories ( ${openBibleViewer_SOURCE_DIR}/src/clucene/shared )
include_directories ( ${openBibleViewer_BINARY_DIR}/src/clucene/shared )
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
    )
add_subdirectory ( src )
add_subdirectory ( src/clucene/core )
add_subdirectory ( src/clucene/shared )
include ( ${QT_USE_FILE} )