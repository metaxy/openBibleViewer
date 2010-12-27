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
#include "src/core/settings/modulecache.h"
#include "src/core/bible/versification/versification_kjv.h"
#include <QtGui/QMessageBox>
#include <QtCore/QLibraryInfo>
#include <QtCore/QMapIterator>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <typeinfo>
#include "src/core/core.h"
#include "config.h"
#include "src/core/version.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_reloadLang = false;
}

MainWindow::~MainWindow()
{
    //DEBUG_FUNC_NAME
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
    delete m_moduleCacheFile;
    m_moduleCacheFile = 0;
    /*delete m_menuBar;
    m_menuBar = 0;*/
    delete m_interface;
    m_interface = 0;
}
void MainWindow::init(const QString &homeDataPath, QSettings *settingsFile)
{
    VERSION  = "0.6.a1";
    BUILD =  "2010-11-8";
    m_homeDataPath = homeDataPath;
    m_settingsFile = settingsFile;
    m_moduleCacheFile = new QSettings(homeDataPath + "moduleCache.ini", QSettings::IniFormat);

    m_moduleManager = new ModuleManager();
    m_bibleDisplay = new BibleDisplay();
    m_settings = new Settings();
    m_notes = new Notes();
    m_session = new Session();
    m_actions = new Actions(this);

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
    //DEBUG_FUNC_NAME
    const QString interface = m_settings->session.getData("interface", QString("advanced")).toString();
    if(interface == "advanced") {
        loadAdvancedInterface();
    } else if(interface == "simple") {
        loadSimpleInterface();
    }
}
void MainWindow::deleteInterface()
{
    //DEBUG_FUNC_NAME
    if(m_interface->hasToolBar()) {
        foreach(QToolBar * bar, m_toolBarList) {
            removeToolBar(bar);
        }
    }

    if(m_interface->hasMenuBar()) {
        delete m_menuBar;
        //todo: remove the docks
    }
    /*
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
        }*/
    //todo: why not?
    /*if(m_interface != 0) {
        delete m_interface;
        m_interface = 0;
    }*/
    delete this->centralWidget();
}
void MainWindow::reloadInterface()
{
    //DEBUG_FUNC_NAME
    deleteInterface();
    loadInterface();
}
void MainWindow::loadSimpleInterface()
{
    //DEBUG_FUNC_NAME
    m_interface = new SimpleInterface(this);
    setAll(m_interface);
    setCentralWidget(m_interface);
    m_interface->init();
    m_interface->createDocks();
    m_interface->createToolBars();
    m_interface->createMenu();


    if(m_interface->hasMenuBar()) {
        m_menuBar = m_interface->menuBar();
        setMenuBar(m_menuBar);
    }
    if(m_interface->hasToolBar()) {
        m_toolBarList = m_interface->toolBars();
        foreach(QToolBar * bar, m_toolBarList) {
            addToolBar(bar);
        }
    }
    connect(this, SIGNAL(settingsChanged(Settings, Settings, bool)), m_interface, SLOT(settingsChanged(Settings, Settings, bool)));
    connect(this, SIGNAL(closing()), m_interface, SLOT(closing()));

    QHashIterator<DockWidget *, Qt::DockWidgetArea> i(m_interface->docks());
    while(i.hasNext()) {
        i.next();
        addDockWidget(i.value(), i.key());
    }
}

void MainWindow::loadAdvancedInterface()
{
    //DEBUG_FUNC_NAME
    m_interface = new AdvancedInterface(this);
    setAll(m_interface);
    m_interface->init();
    m_interface->createDocks();
    m_interface->createToolBars();
    m_interface->createMenu();
    setCentralWidget(m_interface);
    if(m_interface->hasMenuBar()) {
        m_menuBar = m_interface->menuBar();
        setMenuBar(m_menuBar);
    }
    if(m_interface->hasToolBar()) {
        m_toolBarList = m_interface->toolBars();
        foreach(QToolBar * bar, m_toolBarList) {
            addToolBar(bar);
        }
    }
    connect(this, SIGNAL(settingsChanged(Settings, Settings, bool)), m_interface, SLOT(settingsChanged(Settings, Settings, bool)));
    connect(this, SIGNAL(closing()), m_interface, SLOT(closing()));

    QHashIterator<DockWidget *, Qt::DockWidgetArea> i(m_interface->docks());
    while(i.hasNext()) {
        i.next();
        addDockWidget(i.value(), i.key());
    }

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
    //DEBUG_FUNC_NAME
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

    m_settings->defaultVersification = new Versification_KJV();
}
void MainWindow::loadSettings()
{
    //DEBUG_FUNC_NAME
    Version currentVersion(m_settings->version);
    Version settingsVersion(m_settingsFile->value("general/version", m_settings->version).toString());
    //myDebug() << settingsVersion.minorVersion() << currentVersion.minorVersion();
    if(settingsVersion.minorVersion() <= 5 && currentVersion.minorVersion() > settingsVersion.minorVersion()) {
        //change from 0.5 to 0.6
        //remove module cache from openBibleViewer.ini
        m_settingsFile->beginGroup("moduleCache");
        m_settingsFile->remove("");
        m_settingsFile->endGroup();
        //myDebug() << "remove ModuleCache";

    }

    m_settings->encoding = m_settingsFile->value("general/encoding", m_settings->encoding).toString();
    m_settings->zoomstep = m_settingsFile->value("general/zoomstep", m_settings->zoomstep).toInt();
    m_settings->language = m_settingsFile->value("general/language", QLocale::system().name()).toString();
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

    size = m_moduleCacheFile->beginReadArray("modulecache");

    for(int i = 0; i < size; ++i) {
        m_moduleCacheFile->setArrayIndex(i);
        ModuleCache c;
        c.setBookCount(m_moduleCacheFile->value("bookCount").toMap());
        const QStringList names = m_moduleCacheFile->value("bookNames").toStringList();
        const QStringList ids = m_moduleCacheFile->value("bookIDs").toStringList();
        QHash<int, QString> bookNames;
        for(int n = 0; n < ids.size(); ++n) {
            bookNames.insert(ids.at(n).toInt(), names.at(n));
        }
        c.bookNames = bookNames;
        c.title = m_moduleCacheFile->value("title").toString();
        m_settings->m_moduleCache.insert(m_settings->recoverUrl(m_moduleCacheFile->value("path").toString()), c);

    }
    m_moduleCacheFile->endArray();

}
void MainWindow::writeSettings()
{
    //DEBUG_FUNC_NAME

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


    m_moduleCacheFile->beginWriteArray("modulecache");
    QMapIterator<QString, ModuleCache> i(m_settings->m_moduleCache);
    int k = 0;
    while(i.hasNext()) {
        i.next();
        k++;
        m_moduleCacheFile->setArrayIndex(k);
        ModuleCache c = i.value();
        QString url = m_settings->savableUrl(i.key());
        m_moduleCacheFile->setValue("title", c.title);
        QHashIterator<int, QString> i(c.bookNames);
        QStringList bookNames;
        QStringList bookIDs;
        while(i.hasNext()) {
            i.next();
            bookNames << i.value();
            bookIDs << QString::number(i.key());
        }
        m_moduleCacheFile->setValue("bookNames", bookNames);
        m_moduleCacheFile->setValue("bookIDs", bookIDs);
        m_moduleCacheFile->setValue("bookCount", c.toStringMap());
        m_moduleCacheFile->setValue("path", url);
    }
    m_moduleCacheFile->endArray();
}
void MainWindow::saveSettings(Settings newSettings, bool modifedModuleSettings)
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
    emit settingsChanged(oldSettings, newSettings, modifedModuleSettings);
}
void MainWindow::showSettingsDialog(int tabID)
{
    SettingsDialog setDialog(this);
    connect(&setDialog, SIGNAL(settingsChanged(Settings, bool)), this, SLOT(saveSettings(Settings, bool)));
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
    //DEBUG_FUNC_NAME
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
    //DEBUG_FUNC_NAME
    //QMainWindow::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        //myDebug() << "retranslate";
        ui->retranslateUi(this);
        if(m_reloadLang) {
            if(m_interface->hasMenuBar()) {
                m_menuBar = m_interface->menuBar();
                setMenuBar(m_menuBar);
            }
            /*if(m_interface->hasToolBar()) {
                foreach(QToolBar * bar, m_toolBarList) {
                    myDebug() << bar;
                    removeToolBar(bar);
                    myDebug() << "ok";
                }
                foreach(QToolBar * bar, m_interface->toolBars()) {
                    addToolBar(bar);
                }
            }*/
            //todo: ugly but it fix the flickering when opening a file dialog
            m_reloadLang = false;
        }
        break;
    default:
        break;
    }
}
