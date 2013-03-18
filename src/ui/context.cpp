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
#include "src/ui/mainwindow.h"
#include "src/ui/updateschecker.h"

#include "src/core/notes/xmlnotes.h"
#include "src/ui/context.h"

Context::Context(QObject *parent) : QObject(parent)
{
    m_firstStart = false;
}
Context::~Context()
{
    DEBUG_FUNC_NAME
    QMapIterator<int, ModuleSettings*> it(m_settings->m_moduleSettings);
    while(it.hasNext()) {
        it.next();
        if(it.value() != NULL)
            delete it.value();
    }
    m_settings->m_moduleSettings.clear();

    delete m_notes;
    m_notes = nullptr;


    delete m_moduleManager;
    m_moduleManager = nullptr;
    
    delete m_settingsLoader;
    m_settingsLoader = nullptr;
}

void Context::init()
{
    DEBUG_FUNC_NAME
    //create some important folders
    QDir d(m_homeDataPath);
    d.mkpath(m_homeDataPath + "index");
    d.mkpath(m_homeDataPath + "cache");
    d.mkpath(m_homeDataPath + "v11n");
    m_settingsLoader = new SettingsLoader(m_settingsFile, m_homeDataPath);
    m_settings = m_settingsLoader->settings();
    
    //init BasicClass
    m_moduleManager = new ModuleManager();
    m_notes = new XmlNotes();
    m_actions = new Actions();
    
    connect(m_actions, SIGNAL(_toggleFullScreen()), this, SLOT(toggleFullScreen()));

    //first start?
    QFileInfo info(m_settingsFile->fileName());
    if(!info.exists()) {
        m_firstStart = true;
    }

    m_settingsLoader->loadSettings();

    //it is deleting itself when done
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
    m_window = new MainWindow(this);
    setAll(m_window);
#ifdef Q_WS_WIN
    w.setAttribute(Qt::WA_TranslucentBackground);
		w.setAttribute(Qt::WA_NoSystemBackground, false);
		QPalette pal = w.palette();
		QColor bg = pal.window().color();
		bg.setAlpha(180);
		pal.setColor(QPalette::Window, bg);
		w.setPalette(pal);
		w.ensurePolished(); // workaround Oxygen filling the background
		w.setAttribute(Qt::WA_StyledBackground, false);*/

		
	if (QtWin::isCompositionEnabled()) {
		QFile file(":/data/style/win7_transparent.css");
		if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			a.setStyleSheet(in.readAll());
		}

		QFile file2("stylesheet.css");
		if(file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in2(&file2);
			a.setStyleSheet(in2.readAll());
		}
		QtWin::extendFrameIntoClientArea(&w);
		w.setContentsMargins(0, 0, 0, 0);
#endif		
		
    m_window->setTranslator(m_myappTranslator, m_qtTranslator);
    m_window->init(m_firstStart);
    m_window->show();
}
void Context::toggleFullScreen()
{
    if(!m_window->isFullScreen()) {
        m_actions->setFullScreen(true);
        m_window->showFullScreen();
    } else {
        m_actions->setFullScreen(false);
        m_window->showNormal();
    }
}

void Context::setTranslator(QTranslator *my, QTranslator *qt)
{
    m_myappTranslator = my;
    m_qtTranslator = qt;
}
void Context::writeSettings()
{
    m_settingsLoader->saveSettings();
}
