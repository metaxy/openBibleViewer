project( openBibleViewer )
cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR} ${QT_QTWEBKIT_INCLUDE_DIR}
	)
set ( openBibleViewer_HDRS
	src/core/settings.h
	src/module/biblequote.h
	src/module/zefania-bible.h
	src/core/windowcache.h
	src/core/KoXmlReader.h
	src/core/KoXmlWriter.h
	src/core/KoXmlReaderForward.h
	src/core/xbelreader.h
	src/core/xbelwriter.h
	src/module/bible.h
	src/core/chapter.h
	src/core/goto.h
	src/module/zefania-strong.h
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
	src/module/strong.h
	src/core/bibledisplaysettings.h
	src/module/simplemoduleclass.h
	src/core/session.h
	src/core/basicclass.h
	)

set ( openBibleViewer_SRCS
	src/main.cpp
	src/module/biblequote.cpp
	src/module/zefania-bible.cpp
	src/ui/dialog/searchdialog.cpp
	src/ui/dialog/settingsdialog.cpp
	src/ui/dialog/aboutdialog.cpp
	src/core/windowcache.cpp
	src/core/KoXmlReader.cpp
	src/core/KoXmlWriter.cpp
	src/core/notes.cpp
	src/core/xbelreader.cpp
	src/core/xbelwriter.cpp
	src/ui/dialog/searchinfodialog.cpp
	src/module/bible.cpp
	src/ui/interface/advanced/mdiform.cpp
	src/ui/dialog/biblepassagedialog.cpp
	src/ui/dialog/moduleconfigdialog.cpp
	src/core/goto.cpp
	src/module/zefania-strong.cpp
	src/core/history.cpp
	src/ui/dialog/moduledownloaddialog.cpp
	src/core/dbghelper.cpp
	src/core/settings.cpp
	src/core/modulesettings.cpp
	src/core/searchresult.cpp
	src/core/searchquery.cpp
	src/core/searchhit.cpp
	src/core/urlconverter.cpp
	src/core/verseselection.cpp
	src/ui/marklist.cpp
	src/ui/markcategories.cpp
	src/module/modulemanager.cpp
	src/module/module.cpp
	src/module/strong.cpp
	src/ui/dock/strongdockwidget.cpp
	src/ui/dock/searchresultdockwidget.cpp
	src/ui/dock/notesdockwidget.cpp
	src/ui/dock/bookmarksdockwidget.cpp
	src/ui/dock/moduledockwidget.cpp
	src/ui/dock/bookdockwidget.cpp
	src/ui/dock/quickjumpdockwidget.cpp
	src/ui/mainwindow.cpp
	src/core/bibledisplay.cpp
	src/ui/interface/simple/simpleinterface.cpp
	src/ui/interface/advanced/advancedinterface.cpp
	src/ui/interface/study/studyinterface.cpp
	src/ui/interface/interface.cpp
	src/ui/dock/dockwidget.cpp
	src/core/bibledisplaysettings.cpp
	src/module/simplemoduleclass.cpp
	src/ui/noteseditor.cpp
	src/core/session.cpp
	src/ui/dock/moduleproxymodel.cpp
	src/core/simplenotes.cpp
	src/core/basicclass.cpp
	src/ui/highlighter.cpp
	src/ui/dialog/insertlinkdialog.cpp
	)

set ( openBibleViewer_UIS
	src/ui/dialog/searchdialog.ui
	src/ui/dialog/settingsdialog.ui
	src/ui/dialog/searchinfodialog.ui
	src/ui/interface/advanced/mdiform.ui
	src/ui/dialog/biblepassagedialog.ui
	src/ui/dialog/aboutdialog.ui
	src/ui/dialog/moduleconfigdialog.ui
	src/ui/dialog/moduledownloaddialog.ui
	src/ui/marklist.ui
	src/ui/markcategories.ui
	src/ui/dock/strongdockwidget.ui
	src/ui/dock/searchresultdockwidget.ui
	src/ui/dock/notesdockwidget.ui
	src/ui/dock/bookmarksdockwidget.ui
	src/ui/dock/moduledockwidget.ui
	src/ui/dock/bookdockwidget.ui
	src/ui/dock/quickjumpdockwidget.ui
	src/ui/mainwindow.ui
	src/ui/interface/simple/simpleinterface.ui
	src/ui/interface/advanced/advancedinterface.ui
	src/ui/interface/study/studyinterface.ui
	src/ui/noteseditor.ui
	src/ui/dialog/inserthtmldialog.ui
	src/ui/dialog/insertlinkdialog.ui
	)
QT4_WRAP_UI(UIS ${openBibleViewer_UIS})

set ( openBibleViewer_RSCS
	src/icons.qrc
	src/data.qrc
	)
QT4_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})

set ( openBibleViewer_TRS
	src/obv_de.ts
	src/obv_ru.ts
	src/obv_en.ts
	)
QT4_ADD_TRANSLATION(TRS ${openBibleViewer_TRS})

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
	src/ui/dock/strongdockwidget.h
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
	src/ui/dock/moduleproxymodel.h
	src/core/simplenotes.h
	src/ui/highlighter.h
	src/ui/dialog/insertlinkdialog.h
	)
QT4_WRAP_CPP(MOCS ${openBibleViewer_MOCS})

add_executable ( openBibleViewer ${openBibleViewer_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( openBibleViewer  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTWEBKIT_LIBRARY} )