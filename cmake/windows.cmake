#### config paths ####
IF(MSVC)
  #nmake definitions for less output
  #add_definitions( "/nologo /w" )
  #set path to zlib dll
  set( WIN_DEV_DIR "C:\\Users\\paul\\")
  set( WIN_DEV_DIR2 "C:\\\\Users\\\\paul\\\\")
  
  set( MCLUCENE_DIR "${WIN_DEV_DIR2}clucene")
  SET( MCLUCENE_BIN_SHARED   ${MCLUCENE_DIR}\\build\\src\\shared)
  SET( MCLUCENE_SRC_CORE  ${MCLUCENE_DIR}\\src\\core)
  SET( MCLUCENE_SRC_SHARED ${MCLUCENE_DIR}\\src\\shared)
  sddet( MCLUCENE_DLL_SHARED "${MCLUCENE_DIR}\\\\build\\\\bin\\\\Release\\\\clucene-shared.dll")
  set( MCLUCENE_DLL_CORE "${MCLUCENE_DIR}\\\\build\\\\bin\\\\Release\\\\clucene-core.dll")
  set( CL_LIBS "${MCLUCENE_DIR}\\build\\bin\\Release\\clucene-shared.lib" "${MCLUCENE_DIR}\\build\\bin\\Release\\clucene-core.lib")
  
  set( ZLIB_DLL "${WIN_DEV_DIR2}zlib\\\\bin\\\\zlib1.dll")
  set( ZLIB_INCLUDE_DIR "${WIN_DEV_DIR}zlib\\include")
  # no need to change
  set( UNZIP_EXE  "${CMAKE_SOURCE_DIR}\\\\src\\\\extern\\\\7za.exe")
endif() 

add_executable(openBibleViewer WIN32 ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS})
set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS main=main)

target_link_libraries( 
  openBibleViewer
    OBVCore
    RtfReader
    ${QT_LIBRARIES}
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTXML_LIBRARY} 
    ${QT_QTNETWORK_LIBRARY} 
    ${QT_QTWEBKIT_LIBRARY} 
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    ${CL_LIBS}
    ${ZLIB_LIBRARIES}
)

install (
  FILES
    "${QT_LIBRARY_DIR}/../bin/QtCore${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/QtXml${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/QtWebKit${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/QtGui${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/QtNetwork${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/QtScript${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/phonon${QT_VERSION_MAJOR}.dll"
    "${QT_LIBRARY_DIR}/../bin/QtSql${QT_VERSION_MAJOR}.dll"
    "${MCLUCENE_DLL_SHARED}"
    "${MCLUCENE_DLL_CORE}"
    "${UNZIP_EXE}"
    "${ZLIB_DLL}"
  DESTINATION 
    bin
)
    
# CPack

# There is a bug in NSI that does not handle full unix paths properly. Make
# sure there is at least one set of four (4) backlasshes.
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\openBibleViewer.exe")
set(CPACK_PACKAGE_ICON "${WIN_DEV_DIR2}openBibleViewer\\\\src\\\\icons\\\\124x124\\\\openBibleViewer.png")
set(CPACK_NSIS_DISPLAY_NAME "openBibleViewer")
set(CPACK_NSIS_HELP_LINK "http://metaxy.github.com/openBibleViewer")
set(CPACK_NSIS_URL_INFO_ABOUT "http://metaxy.github.com/openBibleViewer")
set(CPACK_NSIS_MODIFY_PATH OFF)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "openBibleViewer")
set(CPACK_GENERATOR "TBZ2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "openBibleViewer Project")
set(CPACK_PACKAGE_VENDOR "Paul Walger")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "8")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(EXECUTABLE "${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")

include(CPack)
include(InstallRequiredSystemLibraries)
include(DeployQt4)
install_qt4_executable("openBibleViewer")