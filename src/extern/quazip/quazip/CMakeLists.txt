# CMake build script, used instead of qmake here.

include(${QT_USE_FILE})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QT_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIRS}
)
ADD_DEFINITIONS(-DQUAZIP_BUILD)
set(RTFQUAZIP_LIB_SOURCES
    JlCompress.cpp
    qioapi.cpp
    quaadler32.cpp
    quacrc32.cpp
    quaziodevice.cpp
    quazip.cpp
    quazipfile.cpp
    quazipnewinfo.cpp
    quazipdir.cpp
    quagzipfile.cpp
    unzip.c
    zip.c
)

# this might only be good on gcc for the STATIC approach
# set(CMAKE_CXX_FLAGS "-fPIC")
#set(CMAKE_C_FLAGS "-fPIC")

# qt4_automoc(${RTFQUAZIP_LIB_SOURCES} quazipfile.h)

set(RTFQUAZIP_MOC_HDRS
#    crypt.h
#    ioapi.h
#    JlCompress.h
#    quaadler32.h
#    quachecksum32.h
#    quacrc32.h
    quazipfile.h
#    quazipfileinfo.h
#    quazip_global.h
#    quazip.h
#    quazipnewinfo.h
)

QT4_WRAP_CPP(RTFQUAZIP_MOC_SRCS ${RTFQUAZIP_MOC_HDRS} )
# add_library(RTFQUAZIP STATIC ${RTFQUAZIP_LIB_SOURCES})
add_library(RTFQUAZIP STATIC ${RTFQUAZIP_LIB_SOURCES} ${RTFQUAZIP_MOC_SRCS})
target_link_libraries(RTFQUAZIP ${QT_QTCORE_LIBRARY})