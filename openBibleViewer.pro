# -------------------------------------------------
# Project created by QtCreator 2009-02-06T07:53:02
# -------------------------------------------------
QT += xml
TARGET = openBibleViewer
TEMPLATE = app
SOURCES += src/main.cpp \
    src/ui/main/mainwindow.cpp \
    src/bible/biblequote.cpp \
    src/bible/zefania.cpp \
    src/ui/searchdialog.cpp \
    src/ui/settingsdialog.cpp \
    src/kernel/tabcache.cpp \
    src/ui/aboutdialog.cpp \
    src/kernel/notes.cpp \
    src/kernel/xbelreader.cpp \
    src/kernel/xbelwriter.cpp \
    src/ui/main/mainnotes.cpp \
    src/ui/main/mainbookmarks.cpp \
    src/ui/main/mainsearch.cpp \
    src/ui/searchinfodialog.cpp \
    src/bible/bible.cpp \
    src/ui/main/mainmdi.cpp \
    src/ui/mdiform.cpp \
    src/ui/poschoser.cpp
HEADERS += src/ui/main/mainwindow.h \
    src/kernel/config.h \
    src/bible/biblequote.h \
    src/bible/zefania.h \
    src/ui/searchdialog.h \
    src/ui/settingsdialog.h \
    src/kernel/tabcache.h \
    src/kernel/stelle.h \
    src/ui/aboutdialog.h \
    src/kernel/notes.h \
    src/kernel/xbelreader.h \
    src/kernel/xbelwriter.h \
    src/ui/searchinfodialog.h \
    src/bible/bible.h \
    src/kernel/chapter.h \
    src/ui/mdiform.h \
    src/ui/poschoser.h
FORMS += src/ui/main/mainwindow.ui \
    src/ui/searchdialog.ui \
    src/ui/settingsdialog.ui \
    src/ui/searchinfodialog.ui \
    src/ui/mdiform.ui \
    src/ui/poschoser.ui \
    src/ui/aboutdialog.ui
RESOURCES += src/icons.qrc
TRANSLATIONS = src/obv_de.ts \
    src/obv_ru.ts
INSTALLS += target

UI_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
OBJECTS_DIR = tmp
