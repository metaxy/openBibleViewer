project( openBibleViewer )
cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )

#build own clucene because no distribution ships clucene >= 0.9.23 
set( BUILD_STATIC_LIBRARIES "On")
set(CLUCENE_VERSION_MAJOR "0")
set(CLUCENE_VERSION_MINOR "9")
set(CLUCENE_VERSION_REVISION "23")
set(CLUCENE_VERSION_PATCH "0")
set(CLUCENE_INT_VERSION 92300)

set(CLUCENE_VERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}.${CLUCENE_VERSION_PATCH}")
set(CLUCENE_SOVERSION "${CLUCENE_VERSION_MAJOR}.${CLUCENE_VERSION_MINOR}.${CLUCENE_VERSION_REVISION}")

include (FindThreads)
if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
else( NOT CMAKE_BUILD_TYPE )
    message( "Compiling as ${CMAKE_BUILD_TYPE}" )
endif( NOT CMAKE_BUILD_TYPE )

option(ENABLE_DEBUG
"enable debug support"
OFF)
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
add_subdirectory ( src/clucene/core )
add_subdirectory ( src/clucene/shared )
include ( ${QT_USE_FILE} )

message ( STATUS "Build static clucene" )


option( _PORTABLE_VERSION "Build a protable version" OFF )

configure_file( ${openBibleViewer_SOURCE_DIR}/src/config.h.cmake 
                ${openBibleViewer_BINARY_DIR}/src/config.h )

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
set ( openBibleViewer_HDRS
	src/core/settings.h
	src/module/biblequote.h
	src/module/zefania-bible.h
	src/core/windowcache.h
	src/core/KoXmlReader.h
	src/core/KoXmlWriter.h
	src/core/KoXmlNS.h
	src/core/KoXmlReaderForward.h
	src/core/xbelreader.h
	src/core/xbelwriter.h
	src/module/bible.h
	src/core/chapter.h
	src/core/goto.h
	src/module/zefania-lex.h
	src/core/history.h
	src/core/dbghelper.h
	src/core/searchquery.h
	src/core/settings.h
	src/core/modulesettings.h
	src/core/searchresult.h
	src/core/searchquery.h
	src/core/searchhit.h
	src/core/urlconverter.h
	src/core/verseselection.h
	src/module/modulemanager.h
	src/module/module.h
	src/core/bibledisplaysettings.h
	src/module/simplemoduleclass.h
	src/core/session.h
	src/core/basicclass.h
	src/module/modulemap.h
	src/module/biblelist.h
	src/core/core.h
	src/core/modulecache.h
	src/module/dictionary.h
	src/core/search.h
	src/module/biblequote-dict.h
	src/core/book.h
	src/core/url.h
	src/core/bibleurl.h
	src/core/faststart.h
    src/
	)

set ( openBibleViewer_SRCS
    src/api/bibleapi.cpp

	src/core/book.cpp
    src/core/url.cpp
    src/core/bibleurl.cpp
    src/core/faststart.cpp
	src/core/windowcache.cpp
	src/core/KoXmlReader.cpp
	src/core/KoXmlWriter.cpp
	src/core/KoXmlNS.cpp
	src/core/notes.cpp
	src/core/xbelreader.cpp
	src/core/xbelwriter.cpp
    src/core/dbghelper.cpp
    src/core/settings.cpp
    src/core/modulesettings.cpp
    src/core/searchresult.cpp
    src/core/searchquery.cpp
    src/core/searchhit.cpp
    src/core/urlconverter.cpp
    src/core/verseselection.cpp
    src/core/search.cpp
    src/core/modulecache.cpp
    src/core/session.cpp
    src/core/simplenotes.cpp
    src/core/basicclass.cpp
    src/core/moduledownloader.cpp
    src/core/bibledisplay.cpp
    src/core/bibledisplaysettings.cpp
    src/core/goto.cpp
    src/core/history.cpp

	src/module/bible.cpp
    src/module/biblequote.cpp
    src/module/zefania-bible.cpp
	src/module/zefania-lex.cpp
	src/module/modulemanager.cpp
	src/module/module.cpp
	src/module/simplemoduleclass.cpp
	src/module/biblelist.cpp
	src/module/modulemap.cpp
	src/module/dictionary.cpp
	src/module/biblequote-dict.cpp
    
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
    src/ui/interface/advanced/mdiform.cpp
    src/ui/interface/advanced/webview.cpp
    src/ui/interface/advanced/advancedinterface.cpp
    src/ui/interface/study/studyinterface.cpp
    src/ui/interface/interface.cpp

    src/ui/mainwindow.cpp
    src/ui/noteseditor.cpp
    src/ui/recursivproxymodel.cpp
    src/ui/highlighter.cpp
    src/ui/marklist.cpp
    src/ui/markcategories.cpp
    src/main.cpp
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
    src/ui/interface/advanced/mdiform.ui
	src/ui/interface/advanced/biblelistwidget.ui

    src/ui/mainwindow.ui
    src/ui/noteseditor.ui
    src/ui/marklist.ui
    src/ui/markcategories.ui

	)
QT4_WRAP_UI(UIS ${openBibleViewer_UIS})

set ( openBibleViewer_RSCS
	src/icons.qrc
	src/data.qrc
	)
QT4_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})

#set ( openBibleViewer_TRS
	#src/locales/obv_de.ts
	#src/locales/obv_ru.ts
	#src/locales/obv_en.ts
	#src/locales/obv_cs.ts
	#)
#QT4_ADD_TRANSLATION(TRS ${openBibleViewer_TRS})

set ( openBibleViewer_MOCS
	src/ui/dialog/searchdialog.h
	src/ui/dialog/settingsdialog.h
	src/ui/dialog/aboutdialog.h
	src/core/notes.h
	src/ui/dialog/searchinfodialog.h
	src/ui/interface/advanced/mdiform.h
	src/ui/dialog/biblepassagedialog.h
	src/ui/dialog/moduleconfigdialog.h
	src/ui/dialog/moduledownloaddialog.h
	src/ui/marklist.h
	src/ui/markcategories.h
	src/ui/dock/dictionarydockwidget.h
	src/ui/dock/searchresultdockwidget.h
	src/ui/dock/notesdockwidget.h
	src/ui/dock/bookmarksdockwidget.h
	src/ui/dock/moduledockwidget.h
	src/ui/dock/bookdockwidget.h
	src/ui/dock/quickjumpdockwidget.h
	src/ui/mainwindow.h
	src/core/bibledisplay.h
	src/ui/interface/simple/simpleinterface.h
	src/ui/interface/advanced/advancedinterface.h
	src/ui/interface/study/studyinterface.h
	src/ui/interface/interface.h
	src/ui/dock/dockwidget.h
	src/ui/noteseditor.h
	src/ui/recursivproxymodel.h
	src/core/simplenotes.h
	src/ui/highlighter.h
	src/ui/dialog/insertlinkdialog.h
	src/core/moduledownloader.h
	src/ui/interface/advanced/webview.h
	src/ui/interface/advanced/biblelistwidget.h
	src/ui/interface/advanced/biblelistitemdelegate.h
    src/ui/interface/advanced/mdiareafilter.h
	src/api/bibleapi.h
	src/ui/dock/advancedsearchresultdockwidget.h
	)
QT4_WRAP_CPP ( MOCS ${openBibleViewer_MOCS} )

add_executable ( openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${MOCS}  )

target_link_libraries ( openBibleViewer 
                        ${QT_QTCORE_LIBRARY} 
                        ${QT_QTGUI_LIBRARY} 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTNETWORK_LIBRARY} 
                        ${QT_QTWEBKIT_LIBRARY} 
                        clucene-core-static 
                        clucene-shared-static)

install ( TARGETS openBibleViewer
         RUNTIME DESTINATION bin )

install ( FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/icons/124x124/openBibleViewer.png"
    DESTINATION "share/pixmaps/" )

install ( FILES "${CMAKE_CURRENT_SOURCE_DIR}/openBibleViewer.desktop"
    DESTINATION "share/applications/" )
