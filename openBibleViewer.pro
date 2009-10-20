# -------------------------------------------------
# Project created by QtCreator 2009-02-06T07:53:02
# -------------------------------------------------
TARGET = openBibleViewer
QT += xml
TEMPLATE = app
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp
SOURCES += src/main.cpp \
    src/ui/main/mainwindow.cpp \
    src/module/biblequote.cpp \
    src/module/zefania-bible.cpp \
    src/ui/searchdialog.cpp \
    src/ui/settingsdialog.cpp \
    src/ui/aboutdialog.cpp \
    src/core/tabcache.cpp \
    src/core/KoXmlReader.cpp \
    src/core/KoXmlWriter.cpp \
    src/core/notes.cpp \
    src/core/xbelreader.cpp \
    src/core/xbelwriter.cpp \
    src/ui/main/mainnotes.cpp \
    src/ui/main/mainbookmarks.cpp \
    src/ui/main/mainsearch.cpp \
    src/ui/searchinfodialog.cpp \
    src/module/bible.cpp \
    src/ui/main/mainmdi.cpp \
    src/ui/mdiform.cpp \
    src/ui/poschoser.cpp \
    src/ui/moduleconfigdialog.cpp \
    src/core/goto.cpp \
    src/module/zefania-strong.cpp \
    src/ui/main/mainstrong.cpp \
    src/core/history.cpp
HEADERS += src/ui/main/mainwindow.h \
    src/core/config.h \
    src/module/biblequote.h \
    src/module/zefania-bible.h \
    src/ui/searchdialog.h \
    src/ui/settingsdialog.h \
    src/core/tabcache.h \
    src/core/stelle.h \
    src/core/KoXmlReader.h \
    src/core/KoXmlWriter.h \
    src/core/KoXmlReaderForward.h \
    src/ui/aboutdialog.h \
    src/core/notes.h \
    src/core/xbelreader.h \
    src/core/xbelwriter.h \
    src/ui/searchinfodialog.h \
    src/module/bible.h \
    src/core/chapter.h \
    src/ui/mdiform.h \
    src/ui/poschoser.h \
    src/core/moduleconfig.h \
    src/ui/moduleconfigdialog.h \
    src/core/goto.h \
    src/module/zefania-strong.h \
    src/core/history.h
FORMS += src/ui/main/mainwindow.ui \
    src/ui/searchdialog.ui \
    src/ui/settingsdialog.ui \
    src/ui/searchinfodialog.ui \
    src/ui/mdiform.ui \
    src/ui/poschoser.ui \
    src/ui/aboutdialog.ui \
    src/ui/moduleconfigdialog.ui
RESOURCES += src/icons.qrc
TRANSLATIONS = src/obv_de.ts \
    src/obv_ru.ts
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
unix {
    INSTALLS += target
}
