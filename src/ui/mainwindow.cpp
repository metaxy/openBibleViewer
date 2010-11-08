/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/ui/dialog/settingsdialog.h"
#include "src/core/dbghelper.h"
#include "src/core/modulecache.h"
#include <QtGui/QMessageBox>
#include <QtCore/QLibraryInfo>
#include <QtCore/QMapIterator>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <typeinfo>
#include "src/core/core.h"
#include "config.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_reloadLang = false;
}

MainWindow::~MainWindow()
{
    DEBUG_FUNC_NAME
    delete ui;
    ui = 0;
    delete m_moduleManager;
    m_moduleManager = 0;
    delete m_bibleDisplay;
    m_bibleDisplay = 0;
    delete m_settings;
    m_settings = 0;
    delete m_notes;
    m_notes = 0;
    delete m_settingsFile;
    m_settingsFile = 0;
    /*delete m_toolBar;
    m_toolBar = 0;*/
    delete m_menuBar;
    m_menuBar = 0;
    delete m_interface;
    m_interface = 0;
}
void MainWindow::init(const QString &homeDataPath, QSettings *settingsFile)
{
    VERSION  = "0.5.3";
    BUILD =  "2010-11-8";
    m_homeDataPath = homeDataPath;
    m_settingsFile = settingsFile;

    m_moduleManager = new ModuleManager();
    m_bibleDisplay = new BibleDisplay();
    m_settings = new Settings();
    m_notes = new Notes();
    m_session = new Session();

    bool firstStart = false;
    QFileInfo info(settingsFile->fileName());
    if(!info.exists()) {
        firstStart = true;
    }
    loadDefaultSettings();
    loadSettings();

    m_moduleManager->setSettings(m_settings);
    m_moduleManager->setNotes(m_notes);
    m_moduleManager->loadAllModules();

    loadInterface();
    restoreSession();
    if(firstStart) {
        QTimer::singleShot(1, this, SLOT(showSettingsDialog_Module()));
    }
}
void MainWindow::loadInterface()
{
    DEBUG_FUNC_NAME
    const QString interface = m_settings->session.getData("interface", QString("advanced")).toString();
    if(interface == "advanced") {
        loadAdvancedInterface();
    } else if(interface == "simple") {
        loadSimpleInterface();
    }
}
void MainWindow::deleteInterface()
{
    DEBUG_FUNC_NAME
    if(m_interface->hasToolBar()) {
        foreach(QToolBar * bar, m_toolBarList)
        removeToolBar(bar);
    }

    if(m_interface->hasMenuBar()) {
        delete m_menuBar;
    }

    if(typeid(*m_interface) == typeid(SimpleInterface)) {
        //myDebug() << "delete simpleInterface";

        if(m_interface->m_moduleDockWidget) {
            removeDockWidget(m_interface->m_moduleDockWidget);
        }
        if(m_interface->m_bookDockWidget) {
            removeDockWidget(m_interface->m_bookDockWidget);
        }
        if(m_interface->m_searchResultDockWidget)
            removeDockWidget(m_interface->m_searchResultDockWidget);

    } else if(typeid(*m_interface) == typeid(AdvancedInterface)) {
        //myDebug() << "delete advacedinterface";
        if(m_interface->m_moduleDockWidget)
            removeDockWidget(m_interface->m_moduleDockWidget);
        if(m_interface->m_bookDockWidget)
            removeDockWidget(m_interface->m_bookDockWidget);
        if(m_interface->m_advancedSearchResultDockWidget)
            removeDockWidget(m_interface->m_advancedSearchResultDockWidget);

        if(m_interface->m_notesDockWidget)
            removeDockWidget(m_interface->m_notesDockWidget);
        if(m_interface->m_bookmarksDockWidget)
            removeDockWidget(m_interface->m_bookmarksDockWidget);
        if(m_interface->m_dictionaryDockWidget)
            removeDockWidget(m_interface->m_dictionaryDockWidget);
        if(m_interface->m_quickJumpDockWidget)
            removeDockWidget(m_interface->m_quickJumpDockWidget);
    }
    //todo: why not?
    /*if(m_interface != 0) {
        delete m_interface;
        m_interface = 0;
    }*/
    delete this->centralWidget();
}
void MainWindow::reloadInterface()
{
    DEBUG_FUNC_NAME
    deleteInterface();
    loadInterface();
}
void MainWindow::loadSimpleInterface()
{
    DEBUG_FUNC_NAME
    m_interface = new SimpleInterface(this);
    setAll(m_interface);

    ModuleDockWidget *moduleDockWidget = new ModuleDockWidget(this);
    m_interface->setModuleDockWidget(moduleDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, moduleDockWidget);

    BookDockWidget *bookDockWidget = new BookDockWidget(this);
    m_interface->setBookDockWidget(bookDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, bookDockWidget);

    SearchResultDockWidget *searchResultDockWidget = new SearchResultDockWidget(this);
    m_interface->setSearchResultDockWidget(searchResultDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, searchResultDockWidget);


    setCentralWidget(m_interface);

    if(m_interface->hasMenuBar()) {
        m_menuBar = m_interface->menuBar();
        setMenuBar(m_menuBar);
    }
    if(m_interface->hasToolBar()) {
        m_toolBarList = m_interface->toolBars();
        foreach(QToolBar * bar, m_toolBarList)
        addToolBar(bar);
    }
    connect(this, SIGNAL(settingsChanged(Settings, Settings)), m_interface, SLOT(settingsChanged(Settings, Settings)));
    connect(this, SIGNAL(closing()), m_interface, SLOT(closing()));
    m_interface->init();
}

void MainWindow::loadAdvancedInterface()
{
    DEBUG_FUNC_NAME
    m_interface = new AdvancedInterface(this);
    setAll(m_interface);

    ModuleDockWidget *moduleDockWidget = new ModuleDockWidget(this);
    m_interface->setModuleDockWidget(moduleDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, moduleDockWidget);

    BookDockWidget *bookDockWidget = new BookDockWidget(this);
    m_interface->setBookDockWidget(bookDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, bookDockWidget);

    AdvancedSearchResultDockWidget *advancedSearchResultDockWidget = new AdvancedSearchResultDockWidget(this);
    m_interface->setAdvancedSearchResultDockWidget(advancedSearchResultDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, advancedSearchResultDockWidget);

    NotesDockWidget *notesDockWidget = new NotesDockWidget(this);
    m_interface->setNotesDockWidget(notesDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, notesDockWidget);

    BookmarksDockWidget *bookmarksDockWidget = new BookmarksDockWidget(this);
    m_interface->setBookmarksDockWidget(bookmarksDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, bookmarksDockWidget);

    DictionaryDockWidget *dictionaryDockWidget = new DictionaryDockWidget(this);
    m_interface->setDictionaryDockWidget(dictionaryDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, dictionaryDockWidget);

    QuickJumpDockWidget *quickJumpDockWidget = new QuickJumpDockWidget(this);
    m_interface->setQuickJumpDockWidget(quickJumpDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, quickJumpDockWidget);


    setCentralWidget(m_interface);
    if(m_interface->hasMenuBar()) {
        m_menuBar = m_interface->menuBar();
        setMenuBar(m_menuBar);
    }
    if(m_interface->hasToolBar()) {
        m_toolBarList = m_interface->toolBars();
        foreach(QToolBar * bar, m_toolBarList)
        addToolBar(bar);
    }
    connect(this, SIGNAL(settingsChanged(Settings, Settings)), m_interface, SLOT(settingsChanged(Settings, Settings)));
    connect(this, SIGNAL(closing()), m_interface, SLOT(closing()));
    m_interface->init();
    QTimer::singleShot(0, m_interface, SLOT(restoreSession()));

}
void MainWindow::loadStudyInterface()
{
    m_interface = new StudyInterface(this);
    setAll(m_interface);
    m_interface->init();
    setCentralWidget(m_interface);
}

void MainWindow::setSettings(Settings set)
{
    *m_settings = set;
}
void MainWindow::loadDefaultSettings()
{
    DEBUG_FUNC_NAME
    m_settings->encoding = "Windows-1251";
    m_settings->zoomstep = 1;
    m_settings->removeHtml = true;
    m_settings->version = VERSION;
    m_settings->build = BUILD;
    m_settings->autoLayout = 1;
    m_settings->onClickBookmarkGo = true;
    m_settings->textFormatting = 0;
    m_settings->homePath = m_homeDataPath;
    //myDebug() << m_settings->homePath;
    m_settings->zefaniaBible_hardCache = true;
    m_settings->zefaniaBible_softCache = true;
    QStringList bookNames;
    bookNames << tr("Genesis");
    bookNames << tr("Exodus");
    bookNames << tr("Leviticus");
    bookNames << tr("Numbers");
    bookNames << tr("Deuteronomy");
    bookNames << tr("Joshua");
    bookNames << tr("Judges");
    bookNames << tr("Ruth");
    bookNames << tr("1 Samuel");
    bookNames << tr("2 Samuel");
    bookNames << tr("1 Kings");
    bookNames << tr("2 Kings");
    bookNames << tr("1 Chronicles");
    bookNames << tr("2 Chronicles");
    bookNames << tr("Ezrav");
    bookNames << tr("Nehemiah");
    bookNames << tr("Esther");
    bookNames << tr("Job");
    bookNames << tr("Psalm");
    bookNames << tr("Proverbs");
    bookNames << tr("Ecclesiastes");
    bookNames << tr("Song of Solomon");
    bookNames << tr("Isaiah");
    bookNames << tr("Jeremiah");
    bookNames << tr("Lamentations");
    bookNames << tr("Ezekiel");
    bookNames << tr("Daniel");
    bookNames << tr("Hosea");
    bookNames << tr("Joel");
    bookNames << tr("Amos");
    bookNames << tr("Obadiah");
    bookNames << tr("Jonah");
    bookNames << tr("Micah");
    bookNames << tr("Nahum");
    bookNames << tr("Habakkuk");
    bookNames << tr("Zephaniah");
    bookNames << tr("Haggai");
    bookNames << tr("Zechariah");
    bookNames << tr("Malachi");
    bookNames << tr("Matthew");
    bookNames << tr("Mark");
    bookNames << tr("Luke");
    bookNames << tr("John");
    bookNames << tr("Acts");
    bookNames << tr("Romans");
    bookNames << tr("1 Corinthians");
    bookNames << tr("2 Corinthians");
    bookNames << tr("Galatians");
    bookNames << tr("Ephesians");
    bookNames << tr("Philippians");
    bookNames << tr("Colossians");
    bookNames << tr("1 Thessalonians");
    bookNames << tr("2 Thessalonians");
    bookNames << tr("1 Timothy");
    bookNames << tr("2 Timothy");
    bookNames << tr("Titus");
    bookNames << tr("Philemon");
    bookNames << tr("Hebrews");
    bookNames << tr("James");
    bookNames << tr("1 Peter");
    bookNames << tr("2 Peter");
    bookNames << tr("1 John");
    bookNames << tr("2 John");
    bookNames << tr("3 John");
    bookNames << tr("Jude");
    bookNames << tr("Revelation");

    QList<QStringList> bookShortNames;
    bookShortNames.append(tr("Gen, Ge, Gn").split(", "));
    bookShortNames.append(tr("Ex, Exo, Exd").split(", "));
    bookShortNames.append(tr("Lev, Le, Lv").split(", "));
    bookShortNames.append(tr("Num, Nu, Nm, Nb").split(", "));
    bookShortNames.append(tr("Deut, Dt").split(", "));
    bookShortNames.append(tr("Josh, Jos, Jsh").split(", "));
    bookShortNames.append(tr("Judg, Jdg, Jg, Jdgs").split(", "));
    bookShortNames.append(tr("Rth, Ru").split(", "));
    bookShortNames.append(tr("1 Sam, 1 Sa, 1Samuel, 1S, I Sa, 1 Sm, 1Sa, I Sam, 1Sam").split(", "));
    bookShortNames.append(tr("2 Sam, 2 Sa, 2S, II Sa, 2 Sm, 2Sa, II Sam, 2Sam").split(", "));
    bookShortNames.append(tr("1 Kgs, 1 Ki, 1K, I Kgs, 1Kgs, I Ki, 1Ki, I Kings, 1Kings, 1st Kgs, 1st Kings, 1Kin").split(", "));
    bookShortNames.append(tr("2 Kgs, 2 Ki, 2K, II Kgs, 2Kgs, II Ki, 2Ki, II Kings, 2Kings, 2nd Kgs, 2nd Kings, 2Kin").split(", "));
    bookShortNames.append(tr("1 Chron, 1 Ch, I Ch, 1Ch, 1 Chr, I Chr, 1Chr, I Chron, 1Chron").split(", "));
    bookShortNames.append(tr("2 Chron, 2 Ch, II Ch, 2Ch, II Chr, 2Chr, II Chron, 2Chron").split(", "));
    bookShortNames.append(tr("Ezra, Ezr").split(", "));
    bookShortNames.append(tr("Neh, Ne").split(", "));
    bookShortNames.append(tr("Esth, Es").split(", "));
    bookShortNames.append(tr("Job, Job, Jb").split(", "));
    bookShortNames.append(tr("Pslm, Ps, Psalms, Psa, Psm, Pss").split(", "));
    bookShortNames.append(tr("Prov, Pr, Prv").split(", "));
    bookShortNames.append(tr("Eccles, Ec, Qoh, Qoheleth").split(", "));
    bookShortNames.append(tr("Song, So, SOS").split(", "));
    bookShortNames.append(tr("Isa, Is").split(", "));
    bookShortNames.append(tr("Jer, Je, Jr").split(", "));
    bookShortNames.append(tr("Lam, La").split(", "));
    bookShortNames.append(tr("Ezek, Eze, Ezk").split(", "));
    bookShortNames.append(tr("Dan, Da, Dn").split(", "));
    bookShortNames.append(tr("Hos, Ho").split(", "));
    bookShortNames.append(tr("Joel, Joe, Jl").split(", "));
    bookShortNames.append(tr("Amos, Am").split(", "));
    bookShortNames.append(tr("Obad, Ob").split(", "));
    bookShortNames.append(tr("Jnh, Jon").split(", "));
    bookShortNames.append(tr("Micah, Mic").split(", "));
    bookShortNames.append(tr("Nah, Na").split(", "));
    bookShortNames.append(tr("Hab, Hab").split(", "));
    bookShortNames.append(tr("Zeph, Zep, Zp").split(", "));
    bookShortNames.append(tr("Haggai, Hag, Hg").split(", "));
    bookShortNames.append(tr("Zech, Zec, Zc").split(", "));
    bookShortNames.append(tr("Mal, Mal, Ml").split(", "));
    bookShortNames.append(tr("Matt, Mt").split(", "));
    bookShortNames.append(tr("Mrk, Mk, Mr").split(", "));
    bookShortNames.append(tr("Luk, Lk").split(", "));
    bookShortNames.append(tr("John, Jn, Jhn").split(", "));
    bookShortNames.append(tr("Acts, Ac").split(", "));
    bookShortNames.append(tr("Rom, Ro, Rm").split(", "));
    bookShortNames.append(tr("1 Cor, 1 Co, I Co, 1Co, I Cor, 1Cor").split(", "));
    bookShortNames.append(tr("2 Cor, 2 Co, II Co, 2Co, II Cor, 2Cor").split(", "));
    bookShortNames.append(tr("Gal, Ga").split(", "));
    bookShortNames.append(tr("Ephes, Eph").split(", "));
    bookShortNames.append(tr("Phil, Php").split(", "));
    bookShortNames.append(tr("Col, Col").split(", "));
    bookShortNames.append(tr("1 Thess, 1 Th, I Th, 1Th, I Thes, 1Thes, I Thess, 1Thess").split(", "));
    bookShortNames.append(tr("2 Thess, 2 Th, II Th, 2Th, II Thes, 2Thes, II Thess, 2Thess").split(", "));
    bookShortNames.append(tr("1 Tim, 1 Ti, I Ti, 1Ti, I Tim, 1Tim").split(", "));
    bookShortNames.append(tr("2 Tim, 2 Ti, II Ti, 2Ti, II Tim, 2Tim").split(", "));
    bookShortNames.append(tr("Titus, Tit").split(", "));
    bookShortNames.append(tr("Philem, Phm").split(", "));
    bookShortNames.append(tr("Hebrews, Heb").split(", "));
    bookShortNames.append(tr("James, Jas, Jm").split(", "));
    bookShortNames.append(tr("1 Pet, 1 Pe, I Pe, 1Pe, I Pet, 1Pet, I Pt, 1 Pt, 1Pt").split(", "));
    bookShortNames.append(tr("2 Pet, 2 Pe, II Pe, 2Pe, II Pet, 2Pet, II Pt, 2 Pt, 2Pt").split(", "));
    bookShortNames.append(tr("1 John, 1 Jn, I Jn, 1Jn, I Jo, 1Jo, I Joh, 1Joh, I Jhn, 1 Jhn, 1Jhn, I John, 1John").split(", "));
    bookShortNames.append(tr("2 John, 2 Jn, II Jn, 2Jn, II Jo, 2Jo, II Joh, 2Joh, II Jhn, 2 Jhn, 2Jhn, II John, 2John").split(", "));
    bookShortNames.append(tr("3 John, 3 Jn, III Jn, 3Jn, III Jo, 3Jo, III Joh, 3Joh, III Jhn, 3 Jhn, 3Jhn, III John, 3John").split(", "));
    bookShortNames.append(tr("Jude, Jud").split(", "));
    bookShortNames.append(tr("Rev, Re, The Revelation").split(", "));
    m_settings->bookShortNames = bookShortNames;
    m_settings->bookFullNames = bookNames;
}
void MainWindow::loadSettings()
{
    DEBUG_FUNC_NAME

    m_settings->encoding = m_settingsFile->value("general/encoding", m_settings->encoding).toString();
    m_settings->zoomstep = m_settingsFile->value("general/zoomstep", m_settings->zoomstep).toInt();
    /*#ifdef Q_WS_WIN
        m_settings->language = m_settingsFile->value("general/language", "en").toString();
    #else*/
    m_settings->language = m_settingsFile->value("general/language", QLocale::system().name()).toString();
    /*#endif*/
    m_settings->autoLayout = m_settingsFile->value("window/layout", m_settings->autoLayout).toInt();
    m_settings->onClickBookmarkGo = m_settingsFile->value("window/onClickBookmarkGo", m_settings->onClickBookmarkGo).toBool();

    m_settings->textFormatting = m_settingsFile->value("bible/textFormatting", m_settings->textFormatting).toInt();

    int size = m_settingsFile->beginReadArray("module");
    for(int i = 0; i < size; ++i) {
        m_settingsFile->setArrayIndex(i);
        ModuleSettings m;
        m.moduleName = m_settingsFile->value("name").toString();
        m.modulePath = m_settings->recoverUrl(m_settingsFile->value("path").toString());
        m.moduleType = m_settingsFile->value("type").toString();
        m.biblequote_removeHtml = m_settingsFile->value("removeHtml", true).toInt();
        m.zefbible_textFormatting = m_settingsFile->value("textFormatting").toInt();
        m.zefbible_hardCache = m_settingsFile->value("hardCache", true).toBool();
        m.zefbible_softCache = m_settingsFile->value("softCache", true).toBool();
        m.zefbible_showStrong = m_settingsFile->value("showStrong", true).toBool();
        m.zefbible_showStudyNote = m_settingsFile->value("showStudyNote", true).toBool();
        m.isDir = m_settingsFile->value("isDir").toBool();
        m.encoding = m_settingsFile->value("encoding").toString();
        m.styleSheet = m_settings->recoverUrl(m_settingsFile->value("styleSheet", ":/data/css/default.css").toString());
        m_settings->m_moduleSettings.append(m);
    }
    m_settingsFile->endArray();
    m_settings->sessionID = m_settingsFile->value("general/lastSession", "0").toString();

    size = m_settingsFile->beginReadArray("sessions");
    for(int i = 0; i < size; ++i) {
        m_settingsFile->setArrayIndex(i);
        Session session;
        if(m_settingsFile->value("id") == m_settings->sessionID) {
            const QStringList keys = m_settingsFile->childKeys();
            for(int j = 0; j < keys.size(); j++) {
                session.setData(keys.at(j), m_settingsFile->value(keys.at(j)));
            }
            m_settings->session = session;//its the current session
        }
        m_settings->sessionIDs.append(m_settingsFile->value("id").toString());
        m_settings->sessionNames.append(m_settingsFile->value("name").toString());

    }
    m_settingsFile->endArray();

    size = m_settingsFile->beginReadArray("modulecache");

    for(int i = 0; i < size; ++i) {
        m_settingsFile->setArrayIndex(i);
        ModuleCache c;
        c.setBookCount(m_settingsFile->value("bookCount").toMap());
        const QStringList names = m_settingsFile->value("bookNames").toStringList();
        const QStringList ids = m_settingsFile->value("bookIDs").toStringList();
        QHash<int, QString> bookNames;
        for(int n = 0; n < ids.size(); ++n) {
            bookNames.insert(ids.at(n).toInt(), names.at(n));
        }
        c.bookNames = bookNames;
        c.title = m_settingsFile->value("title").toString();
        m_settings->m_moduleCache.insert(m_settings->recoverUrl(m_settingsFile->value("path").toString()), c);

    }
    m_settingsFile->endArray();
    return;
}
void MainWindow::writeSettings()
{
    DEBUG_FUNC_NAME
    m_settingsFile->setValue("general/version", m_settings->version);
    m_settingsFile->setValue("general/encoding", m_settings->encoding);
    m_settingsFile->setValue("general/zoomstep", m_settings->zoomstep);
    m_settingsFile->setValue("general/language", m_settings->language);
    m_settingsFile->setValue("general/lastSession", m_settings->sessionID);
    m_settingsFile->setValue("window/layout", m_settings->autoLayout);
    m_settingsFile->setValue("window/onClickBookmarkGo", m_settings->onClickBookmarkGo);
    m_settingsFile->setValue("bible/textFormatting", m_settings->textFormatting);

    m_settingsFile->beginWriteArray("module");
    for(int i = 0; i < m_settings->m_moduleSettings.size(); ++i) {
        m_settingsFile->setArrayIndex(i);
        ModuleSettings m = m_settings->m_moduleSettings.at(i);
        m_settingsFile->setValue("name", m.moduleName);
        m_settingsFile->setValue("path", m_settings->savableUrl(m.modulePath));
        m_settingsFile->setValue("type", m.moduleType);
        m_settingsFile->setValue("textFormatting", m.zefbible_textFormatting);
        m_settingsFile->setValue("removeHtml", m.biblequote_removeHtml);
        m_settingsFile->setValue("hardCache", m.zefbible_hardCache);
        m_settingsFile->setValue("softCache", m.zefbible_softCache);
        m_settingsFile->setValue("showStrong", m.zefbible_showStrong);
        m_settingsFile->setValue("showStudyNote", m.zefbible_showStudyNote);
        m_settingsFile->setValue("isDir", m.isDir);
        m_settingsFile->setValue("encoding", m.encoding);
        m_settingsFile->setValue("styleSheet", m_settings->savableUrl(m.styleSheet));
    }
    m_settingsFile->endArray();

    m_settingsFile->beginWriteArray("sessions");
    m_settingsFile->setArrayIndex(m_settings->sessionIDs.lastIndexOf(m_settings->sessionID));
    {
        QMapIterator <QString, QVariant> i = m_settings->session.getInterator();
        while(i.hasNext()) {
            i.next();
            m_settingsFile->setValue(i.key(), i.value());
        }
    }
    m_settingsFile->endArray();

    m_settingsFile->beginWriteArray("modulecache");
    QMapIterator<QString, ModuleCache> i(m_settings->m_moduleCache);
    int k = 0;
    while(i.hasNext()) {
        i.next();
        k++;
        m_settingsFile->setArrayIndex(k);
        ModuleCache c = i.value();
        QString url = m_settings->savableUrl(i.key());
        m_settingsFile->setValue("title", c.title);
        QHashIterator<int, QString> i(c.bookNames);
        QStringList bookNames;
        QStringList bookIDs;
        while(i.hasNext()) {
            i.next();
            bookNames << i.value();
            bookIDs << QString::number(i.key());
        }
        m_settingsFile->setValue("bookNames", bookNames);
        m_settingsFile->setValue("bookIDs", bookIDs);
        m_settingsFile->setValue("bookCount", c.toStringMap());
        m_settingsFile->setValue("path", url);
    }
    m_settingsFile->endArray();
}
void MainWindow::saveSettings(Settings newSettings)
{
    Settings oldSettings = *m_settings;

    setSettings(newSettings);
    writeSettings();

    if(oldSettings.language != newSettings.language /* || m_settings->theme != set->theme*/) {
        loadLanguage(newSettings.language);
    }
    if(oldSettings.session.getData("interface", "advanced") != newSettings.session.getData("interface", "advanced")) {
        reloadInterface();
    }
    emit settingsChanged(oldSettings, newSettings);
}
void MainWindow::showSettingsDialog(int tabID)
{
    SettingsDialog setDialog(this);
    connect(&setDialog, SIGNAL(settingsChanged(Settings)), this, SLOT(saveSettings(Settings)));
    setDialog.setSettings(*m_settings);
    setDialog.setWindowTitle(tr("Configuration"));
    setDialog.setCurrentTab(tabID);
    setDialog.show();
    setDialog.exec();
}
void MainWindow::showSettingsDialog_Module()
{
    showSettingsDialog(1);
}
void MainWindow::showSettingsDialog_General()
{
    showSettingsDialog(0);
}
void MainWindow::setTranslator(QTranslator *my, QTranslator *qt)
{
    myappTranslator = my;
    qtTranslator = qt;
}
void MainWindow::loadLanguage(QString language)
{
    QStringList avLang;
    //QTranslator myappTranslator;
    QTranslator qtTranslator;
    QString av(_AV_LANG);
    avLang << av.split(";");
    if(avLang.lastIndexOf(language) == -1) {
        language = language.remove(language.lastIndexOf("_"), language.size());
        if(avLang.lastIndexOf(language) == -1) {
            language = avLang.at(0);
        }
    }
    bool loaded = myappTranslator->load(":/data/obv_" + language + ".qm");
    m_reloadLang = true;
    if(!loaded) {
        loaded = myappTranslator->load(language);
        if(!loaded)
            QMessageBox::warning(this, tr("Installing language failed."), tr("Please choose an another language."));
    }

    qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    ui->retranslateUi(this);
}
void MainWindow::restoreSession()
{
    DEBUG_FUNC_NAME
    QByteArray geometry = QVariant(m_settings->session.getData("mainWindowGeometry")).toByteArray();
    QByteArray state = QVariant(m_settings->session.getData("mainWindowState")).toByteArray();
    if(geometry.size() != 0) {
        restoreGeometry(geometry);
    }
    if(state.size() != 0) {
        restoreState(state);
    }
    return;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    m_settings->session.setData("id", m_settings->sessionID);
    m_settings->session.setData("mainWindowGeometry", QVariant(saveGeometry()));
    m_settings->session.setData("mainWindowState", QVariant(saveState()));
    emit closing();
    writeSettings();
}
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        //myDebug() << "retranslate";
        ui->retranslateUi(this);
        if(m_reloadLang) {
            if(m_interface->hasMenuBar()) {
                m_menuBar = m_interface->menuBar();
                setMenuBar(m_menuBar);
            }

            if(m_interface->hasToolBar()) {
                foreach(QToolBar * bar, m_toolBarList)
                removeToolBar(bar);
                if(m_interface->hasToolBar()) {
                    foreach(QToolBar * bar, m_interface->toolBars())
                    addToolBar(bar);
                }
            }
            //todo: ugly but it fix the flickering when opening a file dialog
            m_reloadLang = false;
        }
        break;
    default:
        break;
    }
}
