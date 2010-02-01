# -------------------------------------------------
# Project created by QtCreator 2009-02-06T07:53:02
# -------------------------------------------------
TARGET = openBibleViewer
QT += xml \
    network
TEMPLATE = app
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp
SOURCES += src/main.cpp \
    src/module/biblequote.cpp \
    src/module/zefania-bible.cpp \
    src/ui/dialog/searchdialog.cpp \
    src/ui/dialog/settingsdialog.cpp \
    src/ui/dialog/aboutdialog.cpp \
    src/core/windowcache.cpp \
    src/core/KoXmlReader.cpp \
    src/core/KoXmlWriter.cpp \
    src/core/notes.cpp \
    src/core/xbelreader.cpp \
    src/core/xbelwriter.cpp \
    src/ui/dialog/searchinfodialog.cpp \
    src/module/bible.cpp \
    src/ui/interface/advanced/mdiform.cpp \
    src/ui/dialog/biblepassagedialog.cpp \
    src/ui/dialog/moduleconfigdialog.cpp \
    src/core/goto.cpp \
    src/module/zefania-strong.cpp \
    src/core/history.cpp \
    src/ui/dialog/moduledownloaddialog.cpp \
    src/core/dbghelper.cpp \
    src/core/settings.cpp \
    src/core/modulesettings.cpp \
    src/core/searchresult.cpp \
    src/core/searchquery.cpp \
    src/core/searchhit.cpp \
    src/core/urlconverter.cpp \
    src/core/verseselection.cpp \
    src/ui/noteeditor.cpp \
    src/ui/marklist.cpp \
    src/ui/markcategories.cpp \
    src/module/modulemanager.cpp \
    src/module/module.cpp \
    src/module/strong.cpp \
    src/ui/dock/strongdockwidget.cpp \
    src/ui/dock/searchresultdockwidget.cpp \
    src/ui/dock/notesdockwidget.cpp \
    src/ui/dock/bookmarksdockwidget.cpp \
    src/ui/dock/moduledockwidget.cpp \
    src/ui/dock/bookdockwidget.cpp \
    src/ui/dock/quickjumpdockwidget.cpp \
    src/ui/mainwindow.cpp \ # src/ui/main/mainmdi.cpp \
# src/ui/main/mainbookmarks.cpp \
# src/ui/main/mainnotes.cpp \
# src/ui/main/mainstrong.cpp \
# src/ui/main/mainwindow.cpp \
    src/core/bibledisplay.cpp \
    src/ui/interface/simple/simpleinterface.cpp \
    src/ui/interface/advanced/advancedinterface.cpp \
    src/ui/interface/study/studyinterface.cpp \
    src/ui/interface/interface.cpp \
    src/ui/dock/dockwidget.cpp \
    src/core/bibledisplaysettings.cpp \
    src/module/simplemoduleclass.cpp
HEADERS += src/core/settings.h \
    src/module/biblequote.h \
    src/module/zefania-bible.h \
    src/ui/dialog/searchdialog.h \
    src/ui/dialog/settingsdialog.h \
    src/core/windowcache.h \
    src/core/KoXmlReader.h \
    src/core/KoXmlWriter.h \
    src/core/KoXmlReaderForward.h \
    src/ui/dialog/aboutdialog.h \
    src/core/notes.h \
    src/core/xbelreader.h \
    src/core/xbelwriter.h \
    src/ui/dialog/searchinfodialog.h \
    src/module/bible.h \
    src/core/chapter.h \
    src/ui/interface/advanced/mdiform.h \
    src/ui/dialog/biblepassagedialog.h \
    src/ui/dialog/moduleconfigdialog.h \
    src/core/goto.h \
    src/module/zefania-strong.h \
    src/core/history.h \
    src/ui/dialog/moduledownloaddialog.h \
    src/core/dbghelper.h \
    src/core/searchquery.h \
    src/core/settings.h \
    src/core/modulesettings.h \
    src/core/searchresult.h \
    src/core/searchquery.h \
    src/core/searchhit.h \
    src/core/urlconverter.h \
    src/core/verseselection.h \
    src/ui/noteeditor.h \
    src/ui/marklist.h \
    src/ui/markcategories.h \
    src/module/modulemanager.h \
    src/module/module.h \
    src/module/strong.h \
    src/ui/dock/strongdockwidget.h \
    src/ui/dock/searchresultdockwidget.h \
    src/ui/dock/notesdockwidget.h \
    src/ui/dock/bookmarksdockwidget.h \
    src/ui/dock/moduledockwidget.h \
    src/ui/dock/bookdockwidget.h \
    src/ui/dock/quickjumpdockwidget.h \
    src/ui/mainwindow.h \ # src/ui/main/mainwindow.h \
    src/core/bibledisplay.h \
    src/ui/interface/simple/simpleinterface.h \
    src/ui/interface/advanced/advancedinterface.h \
    src/ui/interface/study/studyinterface.h \
    src/ui/interface/interface.h \
    src/ui/dock/dockwidget.h \
    src/core/bibledisplaysettings.h \
    src/module/simplemoduleclass.h
FORMS += src/ui/dialog/searchdialog.ui \
    src/ui/dialog/settingsdialog.ui \
    src/ui/dialog/searchinfodialog.ui \
    src/ui/interface/advanced/mdiform.ui \
    src/ui/dialog/biblepassagedialog.ui \
    src/ui/dialog/aboutdialog.ui \
    src/ui/dialog/moduleconfigdialog.ui \
    src/ui/dialog/moduledownloaddialog.ui \
    src/ui/noteeditor.ui \
    src/ui/marklist.ui \
    src/ui/markcategories.ui \
    src/ui/dock/strongdockwidget.ui \
    src/ui/dock/searchresultdockwidget.ui \
    src/ui/dock/notesdockwidget.ui \
    src/ui/dock/bookmarksdockwidget.ui \
    src/ui/dock/moduledockwidget.ui \
    src/ui/dock/bookdockwidget.ui \
    src/ui/dock/quickjumpdockwidget.ui \
    src/ui/mainwindow.ui \ # src/ui/main/mainwindow.ui \
    src/ui/interface/simple/simpleinterface.ui \
    src/ui/interface/advanced/advancedinterface.ui \
    src/ui/interface/study/studyinterface.ui
RESOURCES += src/icons.qrc \
    src/data.qrc
TRANSLATIONS = src/obv_de.ts \
    src/obv_ru.ts \
    src/obv_en.ts

# DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
unix:INSTALLS += target
