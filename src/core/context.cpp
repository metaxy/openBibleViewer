/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "context.h"
#include "src/ui/mainwindow.h"
#include "src/core/dbghelper.h"
#include "src/core/verse/versification/versification_kjv.h"
#include "src/ui/updateschecker.h"

#include "src/core/notes/xmlnotes.h"
#include "src/core/notes/textnotes.h"

Context::Context(QObject *parent) : QObject(parent)
{
    m_firstStart = false;
}
Context::~Context()
{
    QMapIterator<int, ModuleSettings*> it(m_settings->m_moduleSettings);
    while(it.hasNext()) {
        it.next();
        if(it.value() != NULL)
            delete it.value();
    }
    m_settings->m_moduleSettings.clear();

    delete m_notes;
    m_notes = NULL;

    delete m_settings;
    m_settings = NULL;

    delete m_settingsFile;
    m_settingsFile = NULL;

    delete m_sessionFile;
    m_sessionFile = NULL;


    delete m_moduleManager;
    m_moduleManager = NULL;
}

void Context::init()
{
    m_sessionFile = new QSettings(m_homeDataPath + "sessions.ini", QSettings::IniFormat);

    //create some important folders
    QDir d(m_homeDataPath);
    d.mkpath(m_homeDataPath + "index");
    d.mkpath(m_homeDataPath + "cache");
    d.mkpath(m_homeDataPath + "v11n");

    m_moduleManager = new ModuleManager();
    m_settings = new Settings();
    m_settings->session.setFile(m_sessionFile);
    m_notes = new XmlNotes();
    m_actions = new Actions();


    QFileInfo info(m_settingsFile->fileName());
    if(!info.exists()) {
        m_firstStart = true;
    }
    loadDefaultSettings();
    loadSettings();

    UpdatesChecker *c = new UpdatesChecker(this);
    setAll(c);
    c->checkForUpdates();

    m_moduleManager->setSettings(m_settings);
    m_moduleManager->setNotes(m_notes);
    m_moduleManager->loadAllModules();

}

void Context::setHomePath(const QString &homePath)
{
    m_homeDataPath = homePath;
}

void Context::setSettings(QSettings *settings)
{
    m_settingsFile = settings;
}
void Context::showWindow()
{
    DEBUG_FUNC_NAME
    MainWindow *w = new MainWindow(this);
    setAll(w);

    w->setTranslator(m_myappTranslator, m_qtTranslator);
    w->init(m_homeDataPath, m_settingsFile, m_firstStart);
    w->show();
}
void Context::setTranslator(QTranslator *my, QTranslator *qt)
{
    m_myappTranslator = my;
    m_qtTranslator = qt;
}
void Context::loadDefaultSettings()
{
    //DEBUG_FUNC_NAME
    m_settings->encoding = "Windows-1251";
    m_settings->zoomstep = 1;
    m_settings->removeHtml = true;
    m_settings->version = QString(OBV_VERSION_NUMBER);
    m_settings->build = QString(OBV_BUILD_DATE);
    m_settings->autoLayout = Settings::Tile;
    m_settings->onClickBookmarkGo = true;
    m_settings->textFormatting = 0;
    m_settings->homePath = m_homeDataPath;
    m_settings->zefaniaBible_hardCache = true;
    m_settings->zefaniaBible_softCache = true;
    m_settings->advancedSearchDock_useCurrentModule = true;

#if !defined(Q_WS_X11)
    m_settings->checkForUpdates = true;
#else
    m_settings->checkForUpdates = false;
#endif

    m_settings->defaultVersification = QSharedPointer<Versification> (new Versification_KJV());
}
void Context::loadSettings()
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
    }

    m_settings->encoding = m_settingsFile->value("general/encoding", m_settings->encoding).toString();
    m_settings->zoomstep = m_settingsFile->value("general/zoomstep", m_settings->zoomstep).toInt();
    m_settings->language = m_settingsFile->value("general/language", QLocale::system().name()).toString();
    m_settings->checkForUpdates = m_settingsFile->value("general/checkForUpdates", m_settings->checkForUpdates).toBool();

    m_settings->autoLayout = (Settings::LayoutEnum) m_settingsFile->value("window/layout", m_settings->autoLayout).toInt();
    m_settings->onClickBookmarkGo = m_settingsFile->value("window/onClickBookmarkGo", m_settings->onClickBookmarkGo).toBool();


    m_settings->textFormatting = m_settingsFile->value("bible/textFormatting", m_settings->textFormatting).toInt();

    int size = m_settingsFile->beginReadArray("module");
    for(int i = 0; i < size; ++i) {
        m_settingsFile->setArrayIndex(i);
        ModuleSettings *m = new ModuleSettings();
        m->moduleID = m_settingsFile->value("id").toInt();

        if(m->moduleID == -1 || m_settingsFile->value("type").toInt() == -1)
            continue;

        m->moduleName = m_settingsFile->value("name").toString();
        m->moduleShortName = m_settingsFile->value("shortName").toString();

        m->modulePath = m_settings->recoverUrl(m_settingsFile->value("path").toString());
        m->moduleType = (ModuleTools::ModuleType) m_settingsFile->value("type").toInt();

        m->moduleLanguage = m_settingsFile->value("language").toString();
        m->encoding = m_settingsFile->value("encoding").toString();

        m->zefbible_textFormatting = (ModuleSettings::ZefBible_TextFormating) m_settingsFile->value("textFormatting").toInt();
        m->zefbible_hardCache = m_settingsFile->value("hardCache", true).toBool();
        m->zefbible_softCache = m_settingsFile->value("softCache", true).toBool();

        m->biblequote_removeHtml = m_settingsFile->value("removeHtml", true).toInt();
        m->defaultModule = (ModuleTools::DefaultModule) m_settingsFile->value("defaultModule", ModuleTools::NotADefaultModule).toInt();
        m->contentType = (ModuleTools::ContentType) m_settingsFile->value("contentType", ModuleTools::UnkownContent).toInt();

        m->styleSheet = m_settings->recoverUrl(m_settingsFile->value("styleSheet", ":/data/css/default.css").toString());

        m->versificationFile = m_settings->recoverUrl(m_settingsFile->value("versificationFile", "").toString());
        m->versificationName = m_settingsFile->value("versificationName", "").toString();
        m->useParentSettings = m_settingsFile->value("useParentSettings", false).toBool();

        m->parentID = m_settingsFile->value("parentID").toInt();

        m->stats_usageCount = m_settingsFile->value("stats/usageCount", 0).toInt();
        m->stats_timesOpend = m_settingsFile->value("stats/timesOpend", 0).toInt();

        QSharedPointer<ModuleDisplaySettings> displaySettings = QSharedPointer<ModuleDisplaySettings>(new ModuleDisplaySettings());
        displaySettings->setShowStudyNotes(m_settingsFile->value("showStudyNotes", true).toBool());
        displaySettings->setShowStrong(m_settingsFile->value("showStrong", true).toBool());
        displaySettings->setShowRefLinks(m_settingsFile->value("showRefLinks", false).toBool());
        displaySettings->setShowNotes(m_settingsFile->value("showNotes", true).toBool());
        displaySettings->setShowMarks(m_settingsFile->value("showMarks", true).toBool());
        displaySettings->setShowBottomToolBar(m_settingsFile->value("showBottomToolBar", true).toBool());
        displaySettings->setShowRMAC(m_settingsFile->value("showRMAC", true).toBool());
        displaySettings->setShowCaptions(m_settingsFile->value("showCaptions", false).toBool());
        displaySettings->setShowProlog(m_settingsFile->value("showProlog", false).toBool());
        displaySettings->setShowStrongInline(m_settingsFile->value("showStrongInline", false).toBool());

        m->setDisplaySettings(displaySettings);

        m_settings->m_moduleSettings.insert(m->moduleID, m);
    }

    m_settingsFile->endArray();
    m_settings->sessionID = m_settingsFile->value("general/lastSession", "0").toString();
    m_settings->session.setID(m_settings->sessionID);

}


void Context::writeSettings()
{
    m_settingsFile->clear();
    m_settingsFile->setValue("general/version", m_settings->version);
    m_settingsFile->setValue("general/encoding", m_settings->encoding);
    m_settingsFile->setValue("general/zoomstep", m_settings->zoomstep);
    m_settingsFile->setValue("general/language", m_settings->language);
    m_settingsFile->setValue("general/checkForUpdates", m_settings->checkForUpdates);
    m_settingsFile->setValue("general/lastSession", m_settings->sessionID);
    m_settingsFile->setValue("window/layout", m_settings->autoLayout);
    m_settingsFile->setValue("window/onClickBookmarkGo", m_settings->onClickBookmarkGo);
    m_settingsFile->setValue("bible/textFormatting", m_settings->textFormatting);
    m_settingsFile->beginWriteArray("module");
    QMapIterator<int, ModuleSettings *> it(m_settings->m_moduleSettings);
    for(int i = 0; it.hasNext(); ++i) {
        it.next();
        m_settingsFile->setArrayIndex(i);
        ModuleSettings *m = it.value();

        if(m->moduleID == -1)
            continue;
        m_settingsFile->setValue("id", m->moduleID);
        m_settingsFile->setValue("name", m->moduleName);
        m_settingsFile->setValue("shortName", m->moduleShortName);
        m_settingsFile->setValue("path", m_settings->savableUrl(m->modulePath));
        m_settingsFile->setValue("type", m->moduleType);
        m_settingsFile->setValue("language", m->moduleLanguage);

        m_settingsFile->setValue("textFormatting", m->zefbible_textFormatting);
        m_settingsFile->setValue("removeHtml", m->biblequote_removeHtml);
        m_settingsFile->setValue("hardCache", m->zefbible_hardCache);
        m_settingsFile->setValue("softCache", m->zefbible_softCache);
        m_settingsFile->setValue("encoding", m->encoding);
        m_settingsFile->setValue("styleSheet", m_settings->savableUrl(m->styleSheet));

        m_settingsFile->setValue("defaultModule", m->defaultModule);
        m_settingsFile->setValue("contentType", m->contentType);
        m_settingsFile->setValue("versificationFile", m_settings->savableUrl(m->versificationFile));
        m_settingsFile->setValue("versificationName", m->versificationName);
        m_settingsFile->setValue("useParentSettings", m->useParentSettings);
        m_settingsFile->setValue("parentID", m->parentID);
        m_settingsFile->setValue("stats/usageCount", m->stats_usageCount);
        m_settingsFile->setValue("stats/timesOpend", m->stats_timesOpend);
        if(!m->useParentSettings) {
            if(m->displaySettings()) {
                ModuleDisplaySettings *displaySettings = m->displaySettings().data();
                m_settingsFile->setValue("showStudyNotes", displaySettings->showStudyNotes());
                m_settingsFile->setValue("showStrong", displaySettings->showStrong());
                m_settingsFile->setValue("showRefLinks", displaySettings->showRefLinks());
                m_settingsFile->setValue("showNotes", displaySettings->showNotes());
                m_settingsFile->setValue("showMarks", displaySettings->showMarks());
                m_settingsFile->setValue("showBottomToolBar", displaySettings->showBottomToolBar());
                m_settingsFile->setValue("showRMAC", displaySettings->showRMAC());
                m_settingsFile->setValue("showCaptions", displaySettings->showCaptions());
                m_settingsFile->setValue("showProlog", displaySettings->showProlog());
                m_settingsFile->setValue("showStrongInline", displaySettings->showStrongInline());
            }
        } else {
            m_settingsFile->remove("showStudyNotes");
            m_settingsFile->remove("showStrong");
            m_settingsFile->remove("showRefLinks");
            m_settingsFile->remove("showNotes");
            m_settingsFile->remove("showMarks");
            m_settingsFile->remove("showBottomToolBar");
            m_settingsFile->remove("showRMAC");
            m_settingsFile->remove("showCaptions");
            m_settingsFile->remove("showProlog");
            m_settingsFile->remove("showStrongsInline");
        }
    }

    m_settingsFile->endArray();
}
