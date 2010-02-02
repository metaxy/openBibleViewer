#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/ui/interface/simple/simpleinterface.h"
#include "src/ui/interface/advanced/advancedinterface.h"
#include "src/ui/interface/study/studyinterface.h"
#include "src/ui/dialog/settingsdialog.h"
#include "src/ui/dock/searchresultdockwidget.h"
#include "src/ui/dock/notesdockwidget.h"
#include "src/core/dbghelper.h"
#include <QtGui/QMessageBox>
#include <QtCore/QLibraryInfo>
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
}
void MainWindow::init(const QString &homeDataPath)
{
    VERSION  = "0.3a1";
    BUILD =  "2009-01-14";
    m_homeDataPath = homeDataPath;

#ifdef Q_WS_WIN
    m_settingsFile = new QSettings(QSettings::IniFormat, QSettings::UserScope, "openBible", "openBibleViewer");
#else
    m_settingsFile = new QSettings(m_homeDataPath + "openBibleViewer.ini", QSettings::IniFormat);
#endif

    myDebug() << "settingsPath = " << m_homeDataPath;


    m_moduleManager = new ModuleManager();
    m_bibleDisplay = new BibleDisplay();
    m_settings = new Settings();

    loadDefaultSettings();
    loadSettings();
    //loadLanguage(m_settings->language);
    m_notes = new Notes("");
    m_moduleManager->setSettings(m_settings);
    m_moduleManager->loadAllModules();
    loadAdvancedInterface();
}
void MainWindow::loadSimpleInterface()
{
    SimpleInterface *simpleInterface = new SimpleInterface(this);
    simpleInterface->setModuleManager(m_moduleManager);
    simpleInterface->setBibleDisplay(m_bibleDisplay);
    simpleInterface->setSettings(m_settings);
    simpleInterface->setNotes(m_notes);

    ModuleDockWidget *moduleDockWidget = new ModuleDockWidget(this);
    simpleInterface->setModuleDockWidget(moduleDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, moduleDockWidget);

    BookDockWidget *bookDockWidget = new BookDockWidget(this);
    simpleInterface->setBookDockWidget(bookDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, bookDockWidget);

    SearchResultDockWidget *searchResultDockWidget = new SearchResultDockWidget(this);
    simpleInterface->setSearchResultDockWidget(searchResultDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, searchResultDockWidget);

    simpleInterface->init();
    setCentralWidget(simpleInterface);
    if (simpleInterface->hasMenuBar())
        setMenuBar(simpleInterface->menuBar());
    if (simpleInterface->hasToolBar())
        addToolBar(simpleInterface->toolBar());
    connect(this, SIGNAL(settingsChanged(Settings)), simpleInterface, SLOT(settingsChanged(Settings)));
    connect(this, SIGNAL(closing()), simpleInterface, SLOT(closing()));
}

void MainWindow::loadAdvancedInterface()
{
    AdvancedInterface *advancedInterface = new AdvancedInterface(this);
    advancedInterface->setModuleManager(m_moduleManager);
    advancedInterface->setBibleDisplay(m_bibleDisplay);
    advancedInterface->setSettings(m_settings);
    advancedInterface->setNotes(m_notes);

    ModuleDockWidget *moduleDockWidget = new ModuleDockWidget(this);
    advancedInterface->setModuleDockWidget(moduleDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, moduleDockWidget);

    BookDockWidget *bookDockWidget = new BookDockWidget(this);
    advancedInterface->setBookDockWidget(bookDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, bookDockWidget);

    SearchResultDockWidget *searchResultDockWidget = new SearchResultDockWidget(this);
    advancedInterface->setSearchResultDockWidget(searchResultDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, searchResultDockWidget);

    NotesDockWidget *notesDockWidget = new NotesDockWidget(this);
    advancedInterface->setNotesDockWidget(notesDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, notesDockWidget);

    BookmarksDockWidget *bookmarksDockWidget = new BookmarksDockWidget(this);
    advancedInterface->setBookmarksDockWidget(bookmarksDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, bookmarksDockWidget);

    StrongDockWidget *strongDockWidget = new StrongDockWidget(this);
    advancedInterface->setStrongDockWidget(strongDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, strongDockWidget);

    advancedInterface->init();
    setCentralWidget(advancedInterface);

    if (advancedInterface->hasMenuBar())
        setMenuBar(advancedInterface->menuBar());
    if (advancedInterface->hasToolBar())
        addToolBar(advancedInterface->toolBar());
    connect(this, SIGNAL(settingsChanged(Settings)), advancedInterface, SLOT(settingsChanged(Settings)));
    connect(this, SIGNAL(closing()), advancedInterface, SLOT(closing()));

}
void MainWindow::loadStudyInterface()
{
    StudyInterface *studyInterface = new StudyInterface(this);
    studyInterface->setModuleManager(m_moduleManager);
    studyInterface->setBibleDisplay(m_bibleDisplay);
    studyInterface->setSettings(m_settings);
    studyInterface->setNotes(m_notes);
    studyInterface->init();
    setCentralWidget(studyInterface);
}
void MainWindow::setSettings(Settings *set)
{
    DEBUG_FUNC_NAME
    m_settings = set;
    return;
}
void MainWindow::setSettings(Settings set)
{
    DEBUG_FUNC_NAME
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
        m.uModuleCount = m_settingsFile->value("uModuleCount").toInt();
        m_settings->module.append(m);

    }
    m_settingsFile->endArray();

    //m_zefStrong.setSettings(m_settings);
    //m_bible.setSettings(m_settings);
    return;
}
void MainWindow::writeSettings()
{
    m_settingsFile->setValue("general/encoding", m_settings->encoding);
    m_settingsFile->setValue("general/zoomstep", m_settings->zoomstep);
    m_settingsFile->setValue("general/language", m_settings->language);
    m_settingsFile->setValue("window/layout", m_settings->autoLayout);
    m_settingsFile->setValue("window/onClickBookmarkGo", m_settings->onClickBookmarkGo);
    m_settingsFile->setValue("bible/textFormatting", m_settings->textFormatting);

    m_settingsFile->beginWriteArray("module");
    for (int i = 0; i < m_settings->module.size(); ++i) {
        m_settingsFile->setArrayIndex(i);
        m_settingsFile->setValue("name", m_settings->module.at(i).moduleName);
        m_settingsFile->setValue("path", m_settings->module.at(i).modulePath);
        m_settingsFile->setValue("type", m_settings->module.at(i).moduleType);
        m_settingsFile->setValue("textFormatting", m_settings->module.at(i).zefbible_textFormatting);
        m_settingsFile->setValue("removeHtml", m_settings->module.at(i).biblequote_removeHtml);
        m_settingsFile->setValue("hardCache", m_settings->module.at(i).zefbible_hardCache);
        m_settingsFile->setValue("softCache", m_settings->module.at(i).zefbible_softCache);
        m_settingsFile->setValue("showStrong", m_settings->module.at(i).zefbible_showStrong);
        m_settingsFile->setValue("showStudyNote", m_settings->module.at(i).zefbible_showStudyNote);
        m_settingsFile->setValue("isDir", m_settings->module.at(i).isDir);
        m_settingsFile->setValue("encoding", m_settings->module.at(i).encoding);
        m_settingsFile->setValue("bookCount", m_settings->module.at(i).bookCount);
        m_settingsFile->setValue("bookNames", m_settings->module.at(i).bookNames);
        m_settingsFile->setValue("biblePath", m_settings->module.at(i).biblePath);
        m_settingsFile->setValue("bibleName", m_settings->module.at(i).bibleName);
        m_settingsFile->setValue("uModuleCount", m_settings->module.at(i).uModuleCount);
    }
    m_settingsFile->endArray();
}
void MainWindow::saveSettings(Settings set)
{
    DEBUG_FUNC_NAME
    myDebug() << "m_settings->language = " << m_settings->language  << " set->language = " << set.language;
    if (m_settings->language != set.language /* || m_settings->theme != set->theme*/) {
        loadLanguage(set.language);
    }
    setSettings(set);
    writeSettings();
    emit settingsChanged(set);
}
void MainWindow::showSettingsDialog(int tabID)
{
    DEBUG_FUNC_NAME
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
    DEBUG_FUNC_NAME
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
    QByteArray geometry = m_settingsFile->value("session/geometry").toByteArray();
    QByteArray state = m_settingsFile->value("session/state").toByteArray();
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
    emit closing();
}
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
