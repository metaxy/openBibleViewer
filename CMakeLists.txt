#set(CMAKE_USER_MAKE_RULES_OVERRIDE
   #${CMAKE_CURRENT_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
#set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
  # ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)

project( openBibleViewer )

cmake_minimum_required( VERSION 2.6 )

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
set( MCLUCENE_DIR "C:\\\\Users\\\\mo\\\\coding\\\\clucene-HEAD-314b755" )
include(InstallRequiredSystemLibraries)
#include(cotire)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Using build type: ${CMAKE_BUILD_TYPE}")

IF(MSVC)
  #nmake definitions for less output
  #add_definitions( "/nologo /w" )
  #set path to zlib dll
  SET(ZLIB_DLL "C:\\\\zlib-1.2.3-lib\\\\zlib1.dll")
endif()

if(UNIX)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions ( -Wall -Wextra -Wformat -g3)
    #add_definitions ( -Wall -Wextra -Wformat -g3 -fstack-check -Weffc++)
  endif()
endif(UNIX)

find_package( Qt4 REQUIRED )
find_package( CLucene1 REQUIRED)


pkg_check_modules(ZLIB REQUIRED zlib)
#find_package( Zlib )

option( OBV_PORTABLE_VERSION "Build a portable version" OFF )
option( OBV_SWORD_SUPPORT "Build with sword support" OFF )

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    option( OBV_NO_DEBUG_OUTPUT "Do not show Debug output" ON )
else()
    option( OBV_NO_DEBUG_OUTPUT "Do not show Debug output" OFF )
endif()

SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG QT_DEBUG)
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG_OUTPUT )
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_WARNING_OUTPUT )
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO QT_NO_DEBUG)
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL QT_NO_DEBUG)
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL QT_NO_DEBUG_OUTPUT )
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL QT_NO_WARNING_OUTPUT )

#we have to use toStdWString() on linux
if(MSVC)
  option( OBV_USE_WSTRING "Use toStdWString and fromWCharArray" OFF)
else()
  option( OBV_USE_WSTRING "Use toStdWString and fromWCharArray" ON)
endif()



if(OBV_SWORD_SUPPORT)
    include (FindSword)
    if (NOT SWORD_NOT_FOUND)
        ADD_DEFINITIONS(-DBUILD_WITH_SWORD)
    else(NOT SWORD_NOT_FOUND)
        set(OBV_SWORD_SUPPORT FALSE)
    endif(NOT SWORD_NOT_FOUND)
endif(OBV_SWORD_SUPPORT)

set(OBV_VERSION_NUMBER "0.7.60")
set(OBV_VERSION_STATUS "unstable")
set(OBV_BUILD_DATE "2012-09-12")
set(QT_USE_QSTRINGBUILDER "True")

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTWEBKIT_INCLUDE_DIR}
    ${openBibleViewer_BINARY_DIR}/src
    ${CLUCENE1_INCLUDE_DIRS}
    ${MCLUCENE_DIR}\\build\\src\\shared
    ${MCLUCENE_DIR}\\src\\core
    ${MCLUCENE_DIR}\\src\\shared
    )

if(OBV_SWORD_SUPPORT)
    include_directories(
        ${SWORD_INCLUDE_DIR}
    )
endif(OBV_SWORD_SUPPORT)
add_subdirectory( src/extern )
add_subdirectory( src/test )
add_subdirectory( src/core )
#Build openBibleViewer

set( openBibleViewer_SRCS
    src/main.cpp
    src/anyoption.cpp

    src/api/moduleapi.cpp
    src/api/notesapi.cpp
    src/api/api.cpp

    src/ui/dialog/searchdialog.cpp
    src/ui/dialog/settingsdialog.cpp
    src/ui/dialog/aboutdialog.cpp
    src/ui/dialog/biblepassagedialog.cpp
    src/ui/dialog/moduleconfigdialog.cpp
    src/ui/dialog/moduledownloaddialog.cpp
    src/ui/dialog/searchinfodialog.cpp
    src/ui/dialog/insertlinkdialog.cpp
    src/ui/dialog/downloadprogressdialog.cpp
    src/ui/dialog/updatecheckerdialog.cpp
    src/ui/dialog/moduleselectdialog.cpp

    src/ui/dock/dockwidget.cpp
    src/ui/dock/dictionarydockwidget.cpp
    src/ui/dock/searchresultdockwidget.cpp
    src/ui/dock/notesdockwidget.cpp
    src/ui/dock/bookmarksdockwidget.cpp
    src/ui/dock/moduledockwidget.cpp
    src/ui/dock/bookdockwidget.cpp
    src/ui/dock/quickjumpdockwidget.cpp
    src/ui/dock/advancedsearchresultdockwidget.cpp
    
    src/ui/interface/simple/simpleinterface.cpp
    src/ui/interface/advanced/versetablewidget.cpp
    src/ui/interface/advanced/versetableitemdelegate.cpp
    src/ui/interface/advanced/mdiarea.cpp
    src/ui/interface/advanced/mdisubwindow.cpp
    src/ui/interface/advanced/form/bibleform.cpp
    src/ui/interface/advanced/form/form.cpp
    src/ui/interface/advanced/form/webform.cpp
    src/ui/interface/advanced/form/dictionaryform.cpp
    src/ui/interface/advanced/form/bookform.cpp
    src/ui/interface/advanced/form/webviewform.cpp
    src/ui/interface/advanced/form/commentaryform.cpp
    src/ui/interface/advanced/form/treebookform.cpp

    src/ui/webview.cpp
    src/ui/interface/advanced/manager/windowmanager.cpp
    src/ui/interface/advanced/manager/bookmarksmanager.cpp
    src/ui/interface/advanced/manager/versemodulemanager.cpp
    src/ui/interface/advanced/manager/dictionarymanager.cpp
    src/ui/interface/advanced/manager/notesmanager.cpp
    src/ui/interface/advanced/manager/searchmanager.cpp
    src/ui/interface/advanced/manager/webpagemanager.cpp
    src/ui/interface/advanced/advancedinterface.cpp
    src/ui/interface/study/studyinterface.cpp
    src/ui/interface/interface.cpp

    src/ui/mainwindow.cpp
    src/ui/noteseditor.cpp
    src/ui/recursivproxymodel.cpp
    src/ui/highlighter.cpp
    src/ui/marklist.cpp
    src/ui/markcategories.cpp
    src/ui/modulemodel.cpp
    src/ui/biblepassageframe.cpp
	src/ui/qtwin.cpp
    src/ui/moduleselectui.cpp

    src/ui/updateschecker.cpp
    
    src/ui/context.cpp

    
    )
if(WIN32)
    set(openBibleViewer_SRCS ${openBibleViewer_SRCS} icon.rc)
endif(WIN32)
 

set ( openBibleViewer_UIS
    src/ui/dialog/searchdialog.ui
    src/ui/dialog/settingsdialog.ui
    src/ui/dialog/searchinfodialog.ui
    src/ui/dialog/biblepassagedialog.ui
    src/ui/dialog/aboutdialog.ui
    src/ui/dialog/moduleconfigdialog.ui
    src/ui/dialog/moduledownloaddialog.ui
    src/ui/dialog/inserthtmldialog.ui
    src/ui/dialog/insertlinkdialog.ui
    src/ui/dialog/downloadprogressdialog.ui
    src/ui/dialog/updatecheckerdialog.ui
    src/ui/dialog/moduleselectdialog.ui

    src/ui/dock/dictionarydockwidget.ui
    src/ui/dock/searchresultdockwidget.ui
    src/ui/dock/notesdockwidget.ui
    src/ui/dock/bookmarksdockwidget.ui
    src/ui/dock/moduledockwidget.ui
    src/ui/dock/bookdockwidget.ui
    src/ui/dock/quickjumpdockwidget.ui
    src/ui/dock/advancedsearchresultdockwidget.ui

    src/ui/interface/simple/simpleinterface.ui
    src/ui/interface/advanced/advancedinterface.ui
    src/ui/interface/study/studyinterface.ui

    src/ui/interface/advanced/form/bibleform.ui
    src/ui/interface/advanced/form/commentaryform.ui
    src/ui/interface/advanced/form/bookform.ui
    src/ui/interface/advanced/form/webform.ui
    src/ui/interface/advanced/form/dictionaryform.ui
    src/ui/interface/advanced/form/treebookform.ui

    src/ui/interface/advanced/versetablewidget.ui

    src/ui/mainwindow.ui
    src/ui/noteseditor.ui
    src/ui/marklist.ui
    src/ui/markcategories.ui
    src/ui/biblepassageframe.ui



    )


set ( openBibleViewer_RSCS
    src/icons.qrc
    src/data.qrc
    )


#set ( openBibleViewer_TRS
    #locales/obv_de.ts
    #locales/obv_ru.ts
    #locales/obv_en.ts
    #locales/obv_cs.ts
    #)
#QT4_ADD_TRANSLATION(TRS ${openBibleViewer_TRS})

set ( openBibleViewer_MOCS
    src/api/moduleapi.h
    src/api/notesapi.h
    src/api/api.h
    
    src/ui/dialog/searchdialog.h
    src/ui/dialog/settingsdialog.h
    src/ui/dialog/aboutdialog.h
    src/ui/dialog/searchinfodialog.h
    src/ui/dialog/biblepassagedialog.h
    src/ui/dialog/moduleconfigdialog.h
    src/ui/dialog/moduledownloaddialog.h
    src/ui/dialog/insertlinkdialog.h
    src/ui/dialog/downloadprogressdialog.h
    src/ui/dialog/updatecheckerdialog.h
    src/ui/dialog/moduleselectdialog.h

    src/ui/marklist.h
    src/ui/markcategories.h
    src/ui/mainwindow.h
    src/ui/highlighter.h
    src/ui/noteseditor.h
    src/ui/recursivproxymodel.h
    src/ui/moduleselectui.h

    src/ui/dock/dictionarydockwidget.h
    src/ui/dock/searchresultdockwidget.h
    src/ui/dock/notesdockwidget.h
    src/ui/dock/bookmarksdockwidget.h
    src/ui/dock/moduledockwidget.h
    src/ui/dock/bookdockwidget.h
    src/ui/dock/quickjumpdockwidget.h
    src/ui/dock/dockwidget.h
    src/ui/dock/advancedsearchresultdockwidget.h

    src/ui/interface/interface.h
    src/ui/interface/simple/simpleinterface.h
    src/ui/interface/study/studyinterface.h

    src/ui/interface/advanced/form/bibleform.h
    src/ui/interface/advanced/form/webform.h
    src/ui/interface/advanced/form/form.h
    src/ui/interface/advanced/form/dictionaryform.h
    src/ui/interface/advanced/form/bookform.h
    src/ui/interface/advanced/form/webviewform.h
    src/ui/interface/advanced/form/commentaryform.h
    src/ui/interface/advanced/form/treebookform.h


    src/ui/interface/advanced/advancedinterface.h
    src/ui/webview.h
    src/ui/interface/advanced/versetablewidget.h
    src/ui/interface/advanced/versetableitemdelegate.h
    src/ui/interface/advanced/mdiarea.h
    src/ui/interface/advanced/mdisubwindow.h
    src/ui/interface/advanced/manager/windowmanager.h
    src/ui/interface/advanced/manager/bookmarksmanager.h
    src/ui/interface/advanced/manager/versemodulemanager.h
    src/ui/interface/advanced/manager/dictionarymanager.h
    src/ui/interface/advanced/manager/notesmanager.h
    src/ui/interface/advanced/manager/searchmanager.h
    src/ui/biblepassageframe.h


    src/ui/updateschecker.h
    src/ui/context.h

    )

configure_file( ${openBibleViewer_SOURCE_DIR}/src/config.h.cmake 
                ${openBibleViewer_BINARY_DIR}/config.h )

QT4_WRAP_CPP( MOCS ${openBibleViewer_MOCS})
QT4_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})
QT4_WRAP_UI(UIS ${openBibleViewer_UIS})

if (MSVC)
    add_executable(openBibleViewer WIN32 ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS})
  
    set_source_files_properties(src/main.cpp PROPERTIES COMPILE_DEFINITIONS main=main)
else ()
  add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS} ${OBVCore_SRCS})
endif ()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	#cotire(openBibleViewer)
endif()


if(MSVC)
    set(CL_LIBS "${MCLUCENE_DIR}\\build\\bin\\clucene-shared.lib" "${MCLUCENE_DIR}\\build\\bin\\clucene-core.lib")
else(MSVC)
    set(CL_LIBS clucene-core)
endif(MSVC)

if(OBV_SWORD_SUPPORT)
    set(SW_LIBS ${CURL_LIBRARIES} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES} ${SWORD_LIBRARY})
else(OBV_SWORD_SUPPORT)
    set(SW_LIBS "")
endif(OBV_SWORD_SUPPORT)

target_link_libraries( openBibleViewer
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
                        ${SW_LIBS}
                        ${ZLIB_LIBRARIES}
)

install( TARGETS openBibleViewer
         RUNTIME DESTINATION bin )

if(UNIX AND NOT WIN32)

    install( FILES "${openBibleViewer_SOURCE_DIR}/src/icons/124x124/openBibleViewer.png"
        DESTINATION "share/pixmaps/" )

    install( FILES "${openBibleViewer_SOURCE_DIR}/openBibleViewer.desktop"
        DESTINATION "share/applications/" )
else(WIN32 AND NOT UNIX)

 install (FILES
        "${QT_LIBRARY_DIR}/../bin/QtCore${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/QtXml${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/QtWebKit${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/QtGui${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/QtNetwork${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/QtScript${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/phonon${QT_VERSION_MAJOR}.dll"
        "${QT_LIBRARY_DIR}/../bin/QtSql${QT_VERSION_MAJOR}.dll"
        "${MCLUCENE_DIR}\\\\build\\\\bin\\\\clucene-shared.dll" 
        "${MCLUCENE_DIR}\\\\build\\\\bin\\\\clucene-core.dll"
        "${ZLIB_DLL}"
        DESTINATION bin
    )
endif(UNIX AND NOT WIN32)

include(InstallRequiredSystemLibraries)


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "openBibleViewer Project")
set(CPACK_PACKAGE_VENDOR "Paul Walger")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "7")
set(CPACK_PACKAGE_VERSION_PATCH "60")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "openBibleViewer 0.8 alpha")
#nsis
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\openBibleViewer.exe")
  SET(CPACK_PACKAGE_ICON "C:\\\\Users\\\\mo\\\\coding\\\\openBibleViewer\\\\src\\\\icons\\\\124x124\\\\openBibleViewer.png")
  set(CPACK_NSIS_DISPLAY_NAME "openBibleViewer")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\openbv.uucyc.name")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\openbv.uucyc.name")
  set(CPACK_NSIS_MODIFY_PATH OFF)
endif(WIN32 AND NOT UNIX)

set(CPACK_PACKAGE_EXECUTABLES "openBibleViewer" "openBibleViewer")
include(CPack)
