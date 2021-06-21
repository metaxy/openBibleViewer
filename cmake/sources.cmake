set( openBibleViewer_SRCS
    src/main.cpp
    src/anyoption.cpp

    src/api/moduleapi.cpp
    src/api/notesapi.cpp
    src/api/searchapi.cpp
    src/api/verseselectionapi.cpp
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
    src/ui/interface/advanced/form/consoleform.cpp

    src/ui/webview.cpp
    src/ui/historylineedit.cpp
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
    src/ui/moduleselectui.cpp
    src/ui/web/networkaccessmanager.cpp
    src/ui/updateschecker.cpp
    
    src/ui/context.cpp
    src/core/blockrules.cpp
    
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
    src/ui/interface/advanced/form/consoleform.ui

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
    src/api/searchapi.h
    src/api/verseselectionapi.h
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
    src/ui/historylineedit.h

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
    src/ui/interface/advanced/form/consoleform.h

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

    src/ui/web/networkaccessmanager.h
    src/ui/updateschecker.h
    src/ui/context.h

    )

configure_file( ${openBibleViewer_SOURCE_DIR}/src/config.h.cmake 
                ${openBibleViewer_BINARY_DIR}/config.h )

QT5_WRAP_CPP( MOCS ${openBibleViewer_MOCS})
QT5_ADD_RESOURCES(RSCS ${openBibleViewer_RSCS})
QT5_WRAP_UI(UIS ${openBibleViewer_UIS})

