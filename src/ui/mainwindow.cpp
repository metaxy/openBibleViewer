/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/ui/dock/notesdockwidget.h"
#include "src/ui/dock/quickjumpdockwidget.h"
#include "src/core/dbghelper.h"
#include <QtGui/QMessageBox>
#include <QtCore/QLibraryInfo>
#include <QtCore/QMapIterator>
#include <QtCore/QTimer>
#include <typeinfo>
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    delete toolBar;
    toolBar = 0;
    delete menuBar;
    menuBar = 0;
    delete m_interface;
    m_interface = 0;
}
void MainWindow::init(const QString &homeDataPath)
{
    VERSION  = "0.4";
    BUILD =  "2010-06-04op";
    m_homeDataPath = homeDataPath;

#ifdef Q_WS_WIN
    m_settingsFile = new QSettings(QSettings::IniFormat, QSettings::UserScope, "openBible", "openBibleViewer");
#else
    m_settingsFile = new QSettings(m_homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#endif


    m_moduleManager = new ModuleManager();
    m_bibleDisplay = new BibleDisplay();
    m_settings = new Settings();
    m_notes = new Notes();
    m_session = new Session();

    loadDefaultSettings();
    loadSettings();
    //loadLanguage(m_settings->language);

    m_moduleManager->setSettings(m_settings);
    m_moduleManager->loadAllModules();
    loadInterface();
    restoreSession();
}
void MainWindow::loadInterface()
{
    DEBUG_FUNC_NAME
    QString interface = m_settings->session.getData("interface").toString();
    myDebug() << "interface = " << interface;
    if(interface == "advanced") {
        loadAdvancedInterface();
    } else if(interface == "simple") {
        loadSimpleInterface();
    } else {
        loadAdvancedInterface();
    }
}
void MainWindow::deleteInterface()
{
    DEBUG_FUNC_NAME
    if(m_interface->hasToolBar()) {
        myDebug() << "removing toolbar bar = " << m_interface->toolBar();
        removeToolBar(toolBar);

    }
    if(m_interface->hasMenuBar()) {
        delete menuBar;
    }


    if(typeid(*m_interface) == typeid(SimpleInterface)) {
        myDebug() << "delete simpleInterface";

        if(m_interface->m_moduleDockWidget) {
            removeDockWidget(m_interface->m_moduleDockWidget);
        }
        if(m_interface->m_bookDockWidget) {
            removeDockWidget(m_interface->m_bookDockWidget);
        }
        if(m_interface->m_searchResultDockWidget)
            removeDockWidget(m_interface->m_searchResultDockWidget);

    } else if(typeid(*m_interface) == typeid(AdvancedInterface)) {
        myDebug() << "delete advacedinterface";
        if(m_interface->m_moduleDockWidget)
            removeDockWidget(m_interface->m_moduleDockWidget);
        if(m_interface->m_bookDockWidget)
            removeDockWidget(m_interface->m_bookDockWidget);
        if(m_interface->m_searchResultDockWidget)
            removeDockWidget(m_interface->m_searchResultDockWidget);

        if(m_interface->m_notesDockWidget)
            removeDockWidget(m_interface->m_notesDockWidget);
        if(m_interface->m_bookmarksDockWidget)
            removeDockWidget(m_interface->m_bookmarksDockWidget);
        if(m_interface->m_strongDockWidget)
            removeDockWidget(m_interface->m_strongDockWidget);
        if(m_interface->m_quickJumpDockWidget)
            removeDockWidget(m_interface->m_quickJumpDockWidget);
    }
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
    m_interface->setModuleManager(m_moduleManager);
    m_interface->setBibleDisplay(m_bibleDisplay);
    m_interface->setSettings(m_settings);
    m_interface->setNotes(m_notes);

    ModuleDockWidget *moduleDockWidget = new ModuleDockWidget(this);
    m_interface->setModuleDockWidget(moduleDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, moduleDockWidget);

    BookDockWidget *bookDockWidget = new BookDockWidget(this);
    m_interface->setBookDockWidget(bookDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, bookDockWidget);

    SearchResultDockWidget *searchResultDockWidget = new SearchResultDockWidget(this);
    m_interface->setSearchResultDockWidget(searchResultDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, searchResultDockWidget);

    m_interface->init();
    setCentralWidget(m_interface);
    if (m_interface->hasMenuBar()) {
        menuBar = m_interface->menuBar();
        setMenuBar(menuBar);
    }
    if (m_interface->hasToolBar())
        toolBar = m_interface->toolBar();
        addToolBar(toolBar);
    connect(this, SIGNAL(settingsChanged(Settings,Settings)), m_interface, SLOT(settingsChanged(Settings,Settings)));
    connect(this, SIGNAL(closing()), m_interface, SLOT(closing()));
}

void MainWindow::loadAdvancedInterface()
{
    DEBUG_FUNC_NAME
    m_interface = new AdvancedInterface(this);
    m_interface->setModuleManager(m_moduleManager);
    m_interface->setBibleDisplay(m_bibleDisplay);
    m_interface->setSettings(m_settings);
    m_interface->setNotes(m_notes);

    ModuleDockWidget *moduleDockWidget = new ModuleDockWidget(this);
    m_interface->setModuleDockWidget(moduleDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, moduleDockWidget);

    BookDockWidget *bookDockWidget = new BookDockWidget(this);
    m_interface->setBookDockWidget(bookDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, bookDockWidget);

    SearchResultDockWidget *searchResultDockWidget = new SearchResultDockWidget(this);
    m_interface->setSearchResultDockWidget(searchResultDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, searchResultDockWidget);

    NotesDockWidget *notesDockWidget = new NotesDockWidget(this);
    m_interface->setNotesDockWidget(notesDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, notesDockWidget);

    BookmarksDockWidget *bookmarksDockWidget = new BookmarksDockWidget(this);
    m_interface->setBookmarksDockWidget(bookmarksDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, bookmarksDockWidget);

    StrongDockWidget *strongDockWidget = new StrongDockWidget(this);
    m_interface->setStrongDockWidget(strongDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, strongDockWidget);

    QuickJumpDockWidget *quickJumpDockWidget = new QuickJumpDockWidget(this);
    m_interface->setQuickJumpDockWidget(quickJumpDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, quickJumpDockWidget);


    setCentralWidget(m_interface);
    myDebug() << "a";
    if (m_interface->hasMenuBar()) {
        menuBar = m_interface->menuBar();
        setMenuBar(menuBar);
    }
    if (m_interface->hasToolBar()) {
        toolBar = m_interface->toolBar();
        addToolBar(toolBar);
    }
    myDebug() << "b";
    connect(this, SIGNAL(settingsChanged(Settings,Settings)), m_interface, SLOT(settingsChanged(Settings, Settings)));
    connect(this, SIGNAL(closing()), m_interface, SLOT(closing()));
    m_interface->init();
    myDebug() << "c";
    QTimer::singleShot(1, m_interface, SLOT(restoreSession()));


}
void MainWindow::loadStudyInterface()
{
    m_interface = new StudyInterface(this);
    m_interface->setModuleManager(m_moduleManager);
    m_interface->setBibleDisplay(m_bibleDisplay);
    m_interface->setSettings(m_settings);
    m_interface->setNotes(m_notes);
    m_interface->init();
    setCentralWidget(m_interface);
}
void MainWindow::setSettings(Settings *set)
{
    //DEBUG_FUNC_NAME
    m_settings = set;
    return;
}
void MainWindow::setSettings(Settings set)
{
    //DEBUG_FUNC_NAME
    *m_settings = set;
    return;
}
void MainWindow::loadDefaultSettings()
{
    m_settings->encoding = "Windows-1251";
    m_settings->zoomstep = 1;
    m_settings->removeHtml = true;
    m_settings->version = VERSION;
    m_settings->build = BUILD;
    m_settings->autoLayout = 1;
    m_settings->onClickBookmarkGo = true;
    m_settings->textFormatting = 0;
    m_settings->homePath = m_homeDataPath;
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
    m_settings->bookNames  = bookNames;
}
void MainWindow::loadSettings()
{
    m_settings->encoding = m_settingsFile->value("general/encoding", m_settings->encoding).toString();
    m_settings->zoomstep = m_settingsFile->value("general/zoomstep", m_settings->zoomstep).toInt();
#ifdef Q_WS_WIN
    m_settings->language = m_settingsFile->value("general/language", "en").toString();
#else
    m_settings->language = m_settingsFile->value("general/language", QLocale::system().name()).toString();
#endif
    m_settings->autoLayout = m_settingsFile->value("window/layout", m_settings->autoLayout).toInt();
    m_settings->onClickBookmarkGo = m_settingsFile->value("window/onClickBookmarkGo", m_settings->onClickBookmarkGo).toBool();

    m_settings->textFormatting = m_settingsFile->value("bible/textFormatting", m_settings->textFormatting).toInt();

    int size = m_settingsFile->beginReadArray("module");
    for (int i = 0; i < size; ++i) {
        m_settingsFile->setArrayIndex(i);
        ModuleSettings m;
        m.moduleName = m_settingsFile->value("name").toString();
        m.modulePath = m_settingsFile->value("path").toString();
        m.moduleType = m_settingsFile->value("type").toString();
        m.biblequote_removeHtml = m_settingsFile->value("removeHtml").toInt();
        m.zefbible_textFormatting = m_settingsFile->value("textFormatting").toInt();
        m.zefbible_hardCache = m_settingsFile->value("hardCache").toBool();
        m.zefbible_softCache = m_settingsFile->value("softCache").toBool();
        m.zefbible_showStrong = m_settingsFile->value("showStrong").toBool();
        m.zefbible_showStudyNote = m_settingsFile->value("showStudyNote").toBool();
        m.isDir = m_settingsFile->value("isDir").toBool();
        m.encoding = m_settingsFile->value("encoding").toString();
        m.bookCount = m_settingsFile->value("bookCount").toMap();
        m.bookNames = m_settingsFile->value("bookNames").toMap();
        m.bibleName = m_settingsFile->value("bibleName").toMap();
        m.biblePath = m_settingsFile->value("biblePath").toMap();
        m.uModuleCount = m_settingsFile->value("uModuleCount",0).toInt();
        m.styleSheet = m_settingsFile->value("styleSheet",":/data/default.css").toInt();
        m_settings->module.append(m);

    }
    m_settingsFile->endArray();
    m_settings->sessionID = m_settingsFile->value("general/lastSession", "0").toString();

    size = m_settingsFile->beginReadArray("sessions");
    for (int i = 0; i < size; ++i) {
        m_settingsFile->setArrayIndex(i);
        Session session;
        if (m_settingsFile->value("id") == m_settings->sessionID) {
            QStringList keys = m_settingsFile->childKeys();
            for (int j = 0; j < keys.size(); j++) {
                session.setData(keys.at(j), m_settingsFile->value(keys.at(j)));
            }
            m_settings->session = session;//its the current session
        }
        m_settings->sessionIDs.append(m_settingsFile->value("id").toString());
        m_settings->sessionNames.append(m_settingsFile->value("name").toString());

    }
    m_settingsFile->endArray();
    return;
}
void MainWindow::writeSettings()
{
    DEBUG_FUNC_NAME
    m_settingsFile->setValue("general/encoding", m_settings->encoding);
    m_settingsFile->setValue("general/zoomstep", m_settings->zoomstep);
    m_settingsFile->setValue("general/language", m_settings->language);
    m_settingsFile->setValue("general/lastSession", m_settings->sessionID);
    m_settingsFile->setValue("window/layout", m_settings->autoLayout);
    m_settingsFile->setValue("window/onClickBookmarkGo", m_settings->onClickBookmarkGo);
    m_settingsFile->setValue("bible/textFormatting", m_settings->textFormatting);

    m_settingsFile->beginWriteArray("module");
    for (int i = 0; i < m_settings->module.size(); ++i) {
        m_settingsFile->setArrayIndex(i);
        ModuleSettings m = m_settings->module.at(i);
        m_settingsFile->setValue("name", m.moduleName);
        m_settingsFile->setValue("path", m.modulePath);
        m_settingsFile->setValue("type", m.moduleType);
        m_settingsFile->setValue("textFormatting", m.zefbible_textFormatting);
        m_settingsFile->setValue("removeHtml", m.biblequote_removeHtml);
        m_settingsFile->setValue("hardCache", m.zefbible_hardCache);
        m_settingsFile->setValue("softCache", m.zefbible_softCache);
        m_settingsFile->setValue("showStrong", m.zefbible_showStrong);
        m_settingsFile->setValue("showStudyNote", m.zefbible_showStudyNote);
        m_settingsFile->setValue("isDir", m.isDir);
        m_settingsFile->setValue("encoding", m.encoding);
        m_settingsFile->setValue("bookCount", m.bookCount);
        m_settingsFile->setValue("bookNames", m.bookNames);
        m_settingsFile->setValue("biblePath", m.biblePath);
        m_settingsFile->setValue("bibleName", m.bibleName);
        m_settingsFile->setValue("uModuleCount", m.uModuleCount);
        m_settingsFile->setValue("styleSheet", m.styleSheet);
    }
    m_settingsFile->endArray();

    m_settingsFile->beginWriteArray("sessions");
    m_settingsFile->setArrayIndex(m_settings->sessionIDs.lastIndexOf(m_settings->sessionID));
    {
        QMapIterator <QString, QVariant> i = m_settings->session.getInterator();
        while (i.hasNext()) {
            i.next();
            m_settingsFile->setValue(i.key(), i.value());
        }
    }
    m_settingsFile->endArray();
}
void MainWindow::saveSettings(Settings newSettings)
{
    //DEBUG_FUNC_NAME
    Settings oldSettings = *m_settings;

    setSettings(newSettings);
    writeSettings();

    if (oldSettings.language != newSettings.language /* || m_settings->theme != set->theme*/) {
        loadLanguage(newSettings.language);
    }
    if( oldSettings.session.getData("interface","advanced") != newSettings.session.getData("interface","advanced")) {
        myDebug() << "loading new interface";
        reloadInterface();
    }
    emit settingsChanged(oldSettings,newSettings);
}
void MainWindow::showSettingsDialog(int tabID)
{
    //DEBUG_FUNC_NAME
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
    //DEBUG_FUNC_NAME
    QStringList avLang;
    //QTranslator myappTranslator;
    QTranslator qtTranslator;
    avLang <<  "en" << "de" << "ru";
    myDebug() << "avLang = " << avLang << " lang = " << language;
    if (avLang.lastIndexOf(language) == -1) {
        language = language.remove(language.lastIndexOf("_"), language.size());
        if (avLang.lastIndexOf(language) == -1) {
            language = avLang.at(0);
        }
    }
    bool loaded = myappTranslator->load(":/data/obv_" + language + ".qm");
    if (loaded == false) {
        QMessageBox::warning(this, tr("Installing Language failed"), tr("Please choose an another language."));
    }

    qtTranslator.load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    ui->retranslateUi(this);
}
void MainWindow::restoreSession()
{
    DEBUG_FUNC_NAME
    QByteArray geometry = QVariant(m_settings->session.getData("mainWindowGeometry")).toByteArray();
    QByteArray state = QVariant(m_settings->session.getData("mainWindowState")).toByteArray();
    if (geometry.size() != 0) {
        restoreGeometry(geometry);
    }
    if (state.size() != 0) {
        restoreState(state);
    }
    return;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    DEBUG_FUNC_NAME
    //save session
    m_settings->session.setData("id", m_settings->sessionID);
    m_settings->session.setData("mainWindowGeometry", QVariant(saveGeometry()));
    m_settings->session.setData("mainWindowState", QVariant(saveState()));
    emit closing();
    writeSettings();
}
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        myDebug() << "retranslate";
        ui->retranslateUi(this);

        if (m_interface->hasMenuBar()) {
            menuBar = m_interface->menuBar();
            setMenuBar(menuBar);
        }

        if (m_interface->hasToolBar()) {
            removeToolBar(toolBar);
            toolBar = m_interface->toolBar();
            addToolBar(toolBar);
        }
        //todo: ugly
        break;
    default:
        break;
    }
}
