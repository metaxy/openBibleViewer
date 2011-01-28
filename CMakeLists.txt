project( openBibleViewer )
cmake_minimum_required( VERSION 2.6 )

set ( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
add_definitions ( -Wall -g)
find_package ( Qt4 REQUIRED )
include (FindThreads)
include (FindZlib)
#include (FindSword)
#LINK_DIRECTORIES(
  #  ${SWORD_LIBRARY_DIR}
#)
if (NOT SWORD_NOT_FOUND)
    ADD_DEFINITIONS(-DBUILD_WITH_SWORD)
endif(NOT SWORD_NOT_FOUND)

option( _PORTABLE_VERSION "Build a portable version" OFF )
option( _SWORD_SUPPORT "Build with sword support" ON )

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
    #${SWORD_INCLUDE_DIR}
    )

add_subdirectory ( src/clucene )
#Build openBibleViewer
set ( openBibleViewer_HDRS

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
    src/core/verse/versification/versification_kjv.h
    src/core/verse/versification/bookv11n.h



    src/core/basicclass.h
    src/core/dbghelper.h
    src/core/goto.h
    src/core/history.h
    src/core/KoXmlReader.h
    src/core/KoXmlWriter.h
    src/core/KoXmlNS.h
    src/core/KoXmlReaderForward.h
    src/core/obvcore.h
    

    src/core/search/searchprovider.h
    src/core/search/searchproviders.h
    src/core/search/search.h
    src/core/search/searchresult.h
    src/core/search/searchquery.h
    src/core/search/searchhit.h
    src/core/settings/settings.h
    src/core/settings/session.h
    src/core/settings/bibledisplaysettings.h
    src/core/settings/modulecache.h
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
    src/module/dictionary/biblequote-dict.h
    src/module/bible/biblemodule.h
    src/module/bible/thewordbible.h
    src/module/bible/swordbible.h

    src/ui/interface/advanced/windowsessiondata.h
    src/ui/modelmodel.h
    )

set ( openBibleViewer_SRCS
    src/main.cpp

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
    src/core/verse/versification/versification_kjv.cpp
    src/core/verse/versification/versification_zefania.cpp
    src/core/verse/versification/versification_biblequote.cpp
    src/core/verse/versification/versification_cache.cpp
    src/core/verse/versification/bookv11n.cpp

    src/core/KoXmlReader.cpp
    src/core/KoXmlWriter.cpp
    src/core/KoXmlNS.cpp
    src/core/notes.cpp
    src/core/dbghelper.cpp
    src/core/verseselection.cpp
    src/core/simplenotes.cpp
    src/core/basicclass.cpp
    src/core/moduledownloader.cpp
    src/core/goto.cpp
    src/core/history.cpp
    src/core/search/search.cpp
    src/core/search/searchprovider.cpp
    src/core/search/searchproviders.cpp
    src/core/search/searchresult.cpp
    src/core/search/searchquery.cpp
    src/core/search/searchhit.cpp
    src/core/settings/settings.cpp
    src/core/settings/modulesettings.cpp
    src/core/settings/modulecache.cpp
    src/core/settings/session.cpp
    src/core/settings/bibledisplaysettings.cpp
    src/core/url.cpp
    src/core/urlconverter.cpp
    src/core/urlconverter2.cpp
    src/core/versereplacer.cpp
    src/core/version.cpp
    src/core/xbelreader.cpp
    src/core/xbelwriter.cpp

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
    src/module/dictionary/biblequote-dict.cpp
    src/module/bible/biblemodule.cpp
    src/module/bible/thewordbible.cpp
    src/module/bible/swordbible.cpp
    
    src/ui/dialog/searchdialog.cpp
    src/ui/dialog/settingsdialog.cpp
    src/ui/dialog/aboutdialog.cpp
    src/ui/dialog/biblepassagedialog.cpp
    src/ui/dialog/moduleconfigdialog.cpp
    src/ui/dialog/moduledownloaddialog.cpp
    src/ui/dialog/searchinfodialog.cpp
    src/ui/dialog/insertlinkdialog.cpp
    
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
    src/ui/interface/advanced/biblelistwidget.cpp
    src/ui/interface/advanced/biblelistitemdelegate.cpp
    src/ui/interface/advanced/mdiareafilter.cpp
    src/ui/interface/advanced/form/bibleform.cpp
    src/ui/interface/advanced/webview.cpp
    src/ui/interface/advanced/manager/windowmanager.cpp
    src/ui/interface/advanced/manager/bookmarksmanager.cpp
    src/ui/interface/advanced/manager/biblemanager.cpp
    src/ui/interface/advanced/manager/notesmanager.cpp
    src/ui/interface/advanced/manager/searchmanager.cpp
    src/ui/interface/advanced/advancedinterface.cpp
    src/ui/interface/advanced/windowsessiondata.cpp
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
    src/ui/interface/advanced/biblelistwidget.ui

    src/ui/mainwindow.ui
    src/ui/noteseditor.ui
    src/ui/marklist.ui
    src/ui/markcategories.ui

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

    src/core/notes.h
    src/core/simplenotes.h
    src/core/moduledownloader.h
    src/core/verse/versification/versification_kjv.h
    src/core/verse/versification/versification_zefania.h
    src/core/verse/versification/versification_biblequote.h
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
    src/ui/interface/advanced/advancedinterface.h
    src/ui/interface/advanced/webview.h
    src/ui/interface/advanced/biblelistwidget.h
    src/ui/interface/advanced/biblelistitemdelegate.h
    src/ui/interface/advanced/mdiareafilter.h
    src/ui/interface/advanced/manager/windowmanager.h
    src/ui/interface/advanced/manager/bookmarksmanager.h
    src/ui/interface/advanced/manager/biblemanager.h
    src/ui/interface/advanced/manager/notesmanager.h
    src/ui/interface/advanced/manager/searchmanager.h
    src/ui/actions.h
    )



configure_file( ${openBibleViewer_SOURCE_DIR}/src/config.h.cmake 
                ${openBibleViewer_BINARY_DIR}/config.h )

QT4_WRAP_CPP ( MOCS ${openBibleViewer_MOCS} )
QT4_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})
QT4_WRAP_UI(UIS ${openBibleViewer_UIS})


add_executable ( openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS}  )

target_link_libraries ( openBibleViewer 
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${QT_QTWEBKIT_LIBRARY} 
                        clucene-core-static 
                        clucene-shared-static
                        ${ZLIB_LIBRARY}
                       # ${CURL_LIBRARIES} # optional, empty if not found
                      #  ${ICU_LIBRARIES} # optional, empty if not found
                      #  ${ICU_I18N_LIBRARIES} # optional, empty if not found
                      ##  ${ZLIB_LIBRARIES} # optional, empty if not found
                       # ${SWORD_LIBRARY}# optional, empty if not found
)


install ( TARGETS openBibleViewer
         RUNTIME DESTINATION bin )

IF(UNIX AND NOT WIN32)
    install ( FILES "${openBibleViewer_SOURCE_DIR}/src/icons/124x124/openBibleViewer.png"
        DESTINATION "share/pixmaps/" )

    install ( FILES "${openBibleViewer_SOURCE_DIR}/openBibleViewer.desktop"
        DESTINATION "share/applications/" ) 
ENDIF(UNIX AND NOT WIN32)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "openBibleViewer Project")
SET(CPACK_PACKAGE_VENDOR "Paul Walger")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "6")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\openBibleViewer.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} opeBibleViewer")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\openbv.uucyc.name")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\openbv.uucyc.name")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/openBibleViewer")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "openBibleViewer" "openBibleViewer")
INCLUDE(CPack)
