configure_file( ${openBibleViewer_SOURCE_DIR}/src/config.h.cmake 
                ${openBibleViewer_BINARY_DIR}/config.h )
include (FindZlib)
set ( openBibleViewer_HDRS

    api/notesapi.h
    api/bibleapi.h
    api/api.h

    core/bible/chapter.h
    core/bible/book.h
    core/bible/bibleurl.h
    core/bible/bibleurlrange.h
    core/bible/bibledisplay.h
    core/bible/booknames.h
    core/bible/range.h
    core/bible/ranges.h
    core/bible/rangeenum.h
    core/bible/textrange.h
    core/bible/textranges.h
    core/bible/verse.h
    core/bible/versification.h
    core/bible/versification/versification_kjv.h


    core/core.h
    core/basicclass.h
    core/dbghelper.h
    core/faststart.h
    core/goto.h
    core/history.h
    core/KoXmlReader.h
    core/KoXmlWriter.h
    core/KoXmlNS.h
    core/KoXmlReaderForward.h
   
    

    core/search/searchprovider.h
    core/search/searchproviders.h
    core/search/search.h
    core/search/searchresult.h
    core/search/searchquery.h
    core/search/searchhit.h
    core/settings/settings.h
    core/settings/session.h
    core/settings/bibledisplaysettings.h
    core/settings/modulecache.h
    core/settings/modulesettings.h

    core/urlconverter.h
    core/versereplacer.h
    core/version.h
    core/verseselection.h
    core/url.h
    core/xbelreader.h
    core/xbelwriter.h

    module/biblequote.h
    module/zefania-bible.h
    module/bible.h
    module/simplemoduleclass.h
    module/modulemanager.h
    module/module.h
    module/zefania-lex.h
    module/modulemap.h
    module/biblelist.h
    module/dictionary.h
    module/biblequote-dict.h
    module/biblemodule.h
    module/thewordbible.h

    ui/interface/advanced/windowsessiondata.h
    )

set ( openBibleViewer_SRCS
    api/bibleapi.cpp
    api/notesapi.cpp
    api/api.cpp

    core/bible/chapter.cpp
    core/bible/book.cpp
    core/bible/bibleurl.cpp
    core/bible/bibleurlrange.cpp
    core/bible/bibledisplay.cpp
    core/bible/booknames.cpp
    core/bible/range.cpp
    core/bible/ranges.cpp
    core/bible/rangeenum.cpp
    core/bible/textrange.cpp
    core/bible/textranges.cpp
    core/bible/verse.cpp
    core/bible/versification.cpp
    core/bible/versification/versification_kjv.cpp

    core/faststart.cpp
    core/KoXmlReader.cpp
    core/KoXmlWriter.cpp
    core/KoXmlNS.cpp
    core/notes.cpp
    core/dbghelper.cpp
    core/verseselection.cpp
    core/simplenotes.cpp
    core/basicclass.cpp
    core/moduledownloader.cpp
    core/goto.cpp
    core/history.cpp
    core/search/search.cpp
    core/search/searchprovider.cpp
    core/search/searchproviders.cpp
    core/search/searchresult.cpp
    core/search/searchquery.cpp
    core/search/searchhit.cpp
    core/settings/settings.cpp
    core/settings/modulesettings.cpp
    core/settings/modulecache.cpp
    core/settings/session.cpp
    core/settings/bibledisplaysettings.cpp
    core/url.cpp
    core/urlconverter.cpp
    core/versereplacer.cpp
    core/version.cpp
    core/xbelreader.cpp
    core/xbelwriter.cpp

    module/bible.cpp
    module/biblequote.cpp
    module/zefania-bible.cpp
    module/zefania-lex.cpp
    module/modulemanager.cpp
    module/module.cpp
    module/simplemoduleclass.cpp
    module/biblelist.cpp
    module/modulemap.cpp
    module/dictionary.cpp
    module/biblequote-dict.cpp
    module/biblemodule.cpp
    module/thewordbible.cpp
    
    ui/dialog/searchdialog.cpp
    ui/dialog/settingsdialog.cpp
    ui/dialog/aboutdialog.cpp
    ui/dialog/biblepassagedialog.cpp
    ui/dialog/moduleconfigdialog.cpp
    ui/dialog/moduledownloaddialog.cpp
    ui/dialog/searchinfodialog.cpp
    ui/dialog/insertlinkdialog.cpp
    
    ui/dock/dockwidget.cpp
    ui/dock/dictionarydockwidget.cpp
    ui/dock/searchresultdockwidget.cpp
    ui/dock/notesdockwidget.cpp
    ui/dock/bookmarksdockwidget.cpp
    ui/dock/moduledockwidget.cpp
    ui/dock/bookdockwidget.cpp
    ui/dock/quickjumpdockwidget.cpp
    ui/dock/advancedsearchresultdockwidget.cpp
    
    ui/interface/simple/simpleinterface.cpp
    ui/interface/advanced/biblelistwidget.cpp
    ui/interface/advanced/biblelistitemdelegate.cpp
    ui/interface/advanced/mdiareafilter.cpp
    ui/interface/advanced/bibleform.cpp
    ui/interface/advanced/webview.cpp
    ui/interface/advanced/windowmanager.cpp
    ui/interface/advanced/biblemanager.cpp
    ui/interface/advanced/notesmanager.cpp
    ui/interface/advanced/searchmanager.cpp
    ui/interface/advanced/advancedinterface.cpp
    ui/interface/advanced/windowsessiondata.cpp
    ui/interface/study/studyinterface.cpp
    ui/interface/interface.cpp

    ui/mainwindow.cpp
    ui/noteseditor.cpp
    ui/recursivproxymodel.cpp
    ui/highlighter.cpp
    ui/marklist.cpp
    ui/markcategories.cpp
    ui/actions.cpp
    main.cpp
	)

set ( openBibleViewer_UIS
    ui/dialog/searchdialog.ui
    ui/dialog/settingsdialog.ui
    ui/dialog/searchinfodialog.ui
    ui/dialog/biblepassagedialog.ui
    ui/dialog/aboutdialog.ui
    ui/dialog/moduleconfigdialog.ui
    ui/dialog/moduledownloaddialog.ui
    ui/dialog/inserthtmldialog.ui
    ui/dialog/insertlinkdialog.ui

    ui/dock/dictionarydockwidget.ui
    ui/dock/searchresultdockwidget.ui
    ui/dock/notesdockwidget.ui
    ui/dock/bookmarksdockwidget.ui
    ui/dock/moduledockwidget.ui
    ui/dock/bookdockwidget.ui
    ui/dock/quickjumpdockwidget.ui
    ui/dock/advancedsearchresultdockwidget.ui

    ui/interface/simple/simpleinterface.ui
    ui/interface/advanced/advancedinterface.ui
    ui/interface/study/studyinterface.ui
    ui/interface/advanced/bibleform.ui
    ui/interface/advanced/biblelistwidget.ui

    ui/mainwindow.ui
    ui/noteseditor.ui
    ui/marklist.ui
    ui/markcategories.ui

	)
QT4_WRAP_UI(UIS ${openBibleViewer_UIS})

set ( openBibleViewer_RSCS
	icons.qrc
	data.qrc
	)
QT4_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})

#set ( openBibleViewer_TRS
	#locales/obv_de.ts
	#locales/obv_ru.ts
	#locales/obv_en.ts
	#locales/obv_cs.ts
	#)
#QT4_ADD_TRANSLATION(TRS ${openBibleViewer_TRS})

set ( openBibleViewer_MOCS
    api/bibleapi.h
    api/notesapi.h
    api/api.h

    core/bible/bibledisplay.h
    core/notes.h
    core/simplenotes.h
    core/moduledownloader.h
    core/bible/versification/versification_kjv.h
    core/bible/versification.h

    ui/dialog/searchdialog.h
    ui/dialog/settingsdialog.h
    ui/dialog/aboutdialog.h
    ui/dialog/searchinfodialog.h
    ui/dialog/biblepassagedialog.h
    ui/dialog/moduleconfigdialog.h
    ui/dialog/moduledownloaddialog.h
    ui/dialog/insertlinkdialog.h

    ui/marklist.h
    ui/markcategories.h
    ui/mainwindow.h
    ui/highlighter.h
    ui/noteseditor.h
    ui/recursivproxymodel.h

    ui/dock/dictionarydockwidget.h
    ui/dock/searchresultdockwidget.h
    ui/dock/notesdockwidget.h
    ui/dock/bookmarksdockwidget.h
    ui/dock/moduledockwidget.h
    ui/dock/bookdockwidget.h
    ui/dock/quickjumpdockwidget.h
    ui/dock/dockwidget.h
    ui/dock/advancedsearchresultdockwidget.h

    ui/interface/interface.h
    ui/interface/simple/simpleinterface.h
    ui/interface/study/studyinterface.h

    ui/interface/advanced/bibleform.h
    ui/interface/advanced/advancedinterface.h
    ui/interface/advanced/webview.h
    ui/interface/advanced/biblelistwidget.h
    ui/interface/advanced/biblelistitemdelegate.h
    ui/interface/advanced/mdiareafilter.h
    ui/interface/advanced/windowmanager.h
    ui/interface/advanced/biblemanager.h
    ui/interface/advanced/notesmanager.h
    ui/interface/advanced/searchmanager.h
    ui/actions.h
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
                        clucene-shared-static
                        ${ZLIB_LIBRARY}
)

install ( TARGETS openBibleViewer
         RUNTIME DESTINATION bin )

install ( FILES "${openBibleViewer_SOURCE_DIR}/src/icons/124x124/openBibleViewer.png"
    DESTINATION "share/pixmaps/" )

install ( FILES "${openBibleViewer_SOURCE_DIR}/openBibleViewer.desktop"
    DESTINATION "share/applications/" ) 
