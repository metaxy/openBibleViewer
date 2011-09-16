#set(CMAKE_USER_MAKE_RULES_OVERRIDE
   #${CMAKE_CURRENT_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
#set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
  # ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)

project( openBibleViewer )

cmake_minimum_required( VERSION 2.6 )

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

include(InstallRequiredSystemLibraries)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

message("-- Using build type: ${CMAKE_BUILD_TYPE}")

IF(MSVC)
  #nmake definitions for less output
  add_definitions( "/nologo /w" )
  #set path to zlib dll
  SET(ZLIB_DLL "C:\\\\zlib-1.2.3-lib\\\\zlib1.dll")
endif()

if(UNIX)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions ( -Wall -g)
  endif()
endif(UNIX)

find_package ( Qt4 REQUIRED )
include(FindPkgConfig)
pkg_check_modules(CLUCENE1 libclucene-core)
find_package(CLucene1)

option( OBV_PORTABLE_VERSION "Build a portable version" OFF )
option( OBV_SWORD_SUPPORT "Build with sword support" ON )

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

#not using "to StdWString" triggers a bad bug on linux
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

set(OBV_VERSION_NUMBER "0.6.50")
set(OBV_VERSION_STATUS "unstable")
set(OBV_BUILD_DATE "2011-08-07")

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
    )

if(OBV_SWORD_SUPPORT)
    include_directories(
        ${SWORD_INCLUDE_DIR}
    )
endif(OBV_SWORD_SUPPORT)

#Build openBibleViewer
set( openBibleViewer_HDRS
    src/anyoption.h

    src/api/notesapi.h
    src/api/bibleapi.h
    src/api/api.h

    src/core/verse/chapter.h
    src/core/verse/book.h
    src/core/verse/verseurl.h
    src/core/verse/verseurlrange.h
    src/core/verse/range.h
    src/core/verse/ranges.h
    src/core/verse/rangeenum.h
    src/core/verse/textrange.h
    src/core/verse/textranges.h
    src/core/verse/verse.h
    src/core/verse/versification.h
    src/core/verse/versificationextendeddata.h
    src/core/verse/versification/versification_kjv.h
    src/core/verse/versification/versification_biblequote.h
    src/core/verse/versification/bookv11n.h
    src/core/verse/reftext.h

    src/core/basicclass.h
    src/core/biblelink.h
    src/core/dbghelper.h
    src/core/history.h
    src/core/KoXmlReader.h
    src/core/KoXmlWriter.h
    src/core/KoXmlNS.h
    src/core/KoXmlReaderForward.h
    src/core/obvcore.h
    src/core/strongurl.h


    src/core/search/searchprovider.h
    src/core/search/searchproviders.h
    src/core/search/search.h
    src/core/search/searchresult.h
    src/core/search/searchquery.h
    src/core/search/searchhit.h
    src/core/settings/settings.h
    src/core/settings/session.h
    src/core/settings/moduledisplaysettings.h
    src/core/settings/modulesettings.h

    src/core/urlconverter.h
    src/core/urlconverter2.h
    src/core/versereplacer.h
    src/core/version.h
    src/core/verseselection.h
    src/core/url.h
    src/core/xbelreader.h
    src/core/xbelwriter.h

    src/module/bible/biblequote.h
    src/module/bible/zefania-bible.h
    src/module/bible/bible.h
    src/module/simplemoduleclass.h
    src/module/modulemanager.h
    src/module/module.h
    src/module/versemodule.h
    src/module/dictionary/zefania-lex.h
    src/module/modulemap.h
    src/module/versetable.h
    src/module/dictionary/dictionary.h
    src/module/dictionary/webdictionary.h
    src/module/dictionary/biblequote-dict.h
    src/module/dictionary/dictionarymodule.h
    src/module/bible/biblemodule.h
    src/module/bible/thewordbible.h
    src/module/bible/swordbible.h
    src/module/metainfo.h
    src/module/searchablemodule.h
    src/module/webpage.h

   src/ui/interface/advanced/manager/webpagemanager.h
    src/ui/modelmodel.h
    )

set( openBibleViewer_SRCS
    src/main.cpp
    src/anyoption.cpp

    src/api/bibleapi.cpp
    src/api/notesapi.cpp
    src/api/api.cpp

    src/core/verse/chapter.cpp
    src/core/verse/book.cpp
    src/core/verse/verseurl.cpp
    src/core/verse/verseurlrange.cpp
    src/core/verse/range.cpp
    src/core/verse/ranges.cpp
    src/core/verse/rangeenum.cpp
    src/core/verse/textrange.cpp
    src/core/verse/textranges.cpp
    src/core/verse/verse.cpp
    src/core/verse/versification.cpp
    src/core/verse/versificationextendeddata.cpp
    src/core/verse/versification/versification_kjv.cpp
    src/core/verse/versification/versification_zefania.cpp
    src/core/verse/versification/versification_biblequote.cpp
    src/core/verse/versification/versification_cache.cpp
    src/core/verse/versification/bookv11n.cpp
    src/core/verse/reftext.cpp

    src/core/KoXmlReader.cpp
    src/core/KoXmlWriter.cpp
    src/core/KoXmlNS.cpp
    src/core/notes/notes.cpp
    src/core/notes/xmlnotes.cpp
    src/core/notes/textnotes.cpp
    src/core/dbghelper.cpp
    src/core/verseselection.cpp
    src/core/simplenotes.cpp
    src/core/basicclass.cpp
    src/core/biblelink.cpp
    src/core/moduledownloader.cpp
    src/core/history.cpp
    src/core/strongurl.cpp
    src/core/search/search.cpp
    src/core/search/searchprovider.cpp
    src/core/search/searchproviders.cpp
    src/core/search/searchresult.cpp
    src/core/search/searchquery.cpp
    src/core/search/searchhit.cpp
    src/core/settings/settings.cpp
    src/core/settings/modulesettings.cpp
    src/core/settings/session.cpp
    src/core/settings/moduledisplaysettings.cpp
    src/core/url.cpp
    src/core/urlconverter.cpp
    src/core/urlconverter2.cpp
    src/core/versereplacer.cpp
    src/core/version.cpp
    src/core/xbelreader.cpp
    src/core/xbelwriter.cpp
    src/core/downloadinfile.cpp

    src/module/bible/bible.cpp
    src/module/bible/biblequote.cpp
    src/module/bible/zefania-bible.cpp
    src/module/dictionary/zefania-lex.cpp
    src/module/modulemanager.cpp
    src/module/module.cpp
    src/module/versemodule.cpp
    src/module/simplemoduleclass.cpp
    src/module/versetable.cpp
    src/module/modulemap.cpp
    src/module/dictionary/dictionary.cpp
    src/module/dictionary/webdictionary.cpp
    src/module/dictionary/dictionarymodule.cpp
    src/module/dictionary/biblequote-dict.cpp
    src/module/bible/biblemodule.cpp
    src/module/bible/thewordbible.cpp
    src/module/bible/swordbible.cpp
    src/module/metainfo.cpp
    src/module/searchablemodule.cpp
    src/module/webpage.cpp
    
    src/ui/dialog/searchdialog.cpp
    src/ui/dialog/settingsdialog.cpp
    src/ui/dialog/aboutdialog.cpp
    src/ui/dialog/biblepassagedialog.cpp
    src/ui/dialog/moduleconfigdialog.cpp   src/ui/interface/advanced/manager/webpagemanager.cpp
    src/ui/dialog/moduledownloaddialog.cpp
    src/ui/dialog/searchinfodialog.cpp
    src/ui/dialog/insertlinkdialog.cpp
    src/ui/dialog/downloadprogressdialog.cpp
    src/ui/dialog/updatecheckerdialog.cpp
    
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
    src/ui/interface/advanced/mdiareafilter.cpp
    src/ui/interface/advanced/form/bibleform.cpp
    src/ui/interface/advanced/form/form.cpp
    src/ui/interface/advanced/form/webform.cpp
    src/ui/interface/advanced/form/dictionaryform.cpp

    src/ui/interface/advanced/webview.cpp
    src/ui/interface/advanced/manager/windowmanager.cpp
    src/ui/interface/advanced/manager/bookmarksmanager.cpp
    src/ui/interface/advanced/manager/biblemanager.cpp
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
    src/ui/actions.cpp
    src/ui/modulemodel.cpp
    src/ui/biblepassageframe.cpp

    src/ui/updateschecker.cpp
    
    )

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
    src/ui/interface/advanced/form/webform.ui
    src/ui/interface/advanced/form/dictionaryform.ui

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
    src/api/bibleapi.h
    src/api/notesapi.h
    src/api/api.h

    src/core/notes/notes.h
    src/core/notes/xmlnotes.h
    src/core/notes/textnotes.h
    src/core/downloadinfile.h
    src/core/simplenotes.h
    src/core/moduledownloader.h
    src/core/verse/versification/versification_kjv.h
    src/core/verse/versification/versification_zefania.h
    src/core/verse/versification/versification_cache.h
    src/core/verse/versification.h

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

    src/ui/marklist.h
    src/ui/markcategories.h
    src/ui/mainwindow.h
    src/ui/highlighter.h
    src/ui/noteseditor.h
    src/ui/recursivproxymodel.h

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

    src/ui/interface/advanced/advancedinterface.h
    src/ui/interface/advanced/webview.h
    src/ui/interface/advanced/versetablewidget.h
    src/ui/interface/advanced/versetableitemdelegate.h
    src/ui/interface/advanced/mdiareafilter.h
    src/ui/interface/advanced/manager/windowmanager.h
    src/ui/interface/advanced/manager/bookmarksmanager.h
    src/ui/interface/advanced/manager/biblemanager.h
    src/ui/interface/advanced/manager/dictionarymanager.h
    src/ui/interface/advanced/manager/notesmanager.h
    src/ui/interface/advanced/manager/searchmanager.h
    src/ui/actions.h
    src/ui/biblepassageframe.h


    src/ui/updateschecker.h

    )

configure_file( ${openBibleViewer_SOURCE_DIR}/src/config.h.cmake 
                ${openBibleViewer_BINARY_DIR}/config.h )

QT4_WRAP_CPP( MOCS ${openBibleViewer_MOCS} )
QT4_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})
QT4_WRAP_UI(UIS ${openBibleViewer_UIS})

if (MSVC)
  add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS})
else ()
  add_executable(openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS})
endif ()

if(MSVC)
    set(CL_LIBS  "${CLUCENE_DIR}\\build\\bin\\clucene-shared.lib" "${CLUCENE_DIR}\\build\\bin\\clucene-core.lib")
else(MSVC)
    set(CL_LIBS clucene-core)
endif(MSVC)

if(OBV_SWORD_SUPPORT)
    set(SW_LIBS ${CURL_LIBRARIES} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES} ${ZLIB_LIBRARIES} ${SWORD_LIBRARY})
else(OBV_SWORD_SUPPORT)
    set(SW_LIBS "")
endif(OBV_SWORD_SUPPORT)


target_link_libraries( openBibleViewer
			${QT_LIBRARIES}
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${QT_QTWEBKIT_LIBRARY} 
                        ${QT_QTSCRIPT_LIBRARY}
                        ${CL_LIBS}
                        ${SW_LIBS}
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
	"${QT_LIBRARY_DIR}/../bin/phonon${QT_VERSION_MAJOR}.dll"
	"${CLUCENE_DIR}\\\\build\\\\bin\\\\clucene-shared.dll" 
	"${CLUCENE_DIR}\\\\build\\\\bin\\\\clucene-core.dll"
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
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_PACKAGE_VERSION_PATCH "50")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "openBibleViewer ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
#nsis
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  # SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\openBibleViewer.exe")
  set(CPACK_NSIS_DISPLAY_NAME "openBibleViewer")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\openbv.uucyc.name")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\openbv.uucyc.name")
  set(CPACK_NSIS_MODIFY_PATH OFF)
endif(WIN32 AND NOT UNIX)

set(CPACK_PACKAGE_EXECUTABLES "openBibleViewer" "openBibleViewer")
include(CPack)
