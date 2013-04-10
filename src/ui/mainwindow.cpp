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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/core/module/modulemanager.h"
#include <QPointer>

#include "src/ui/interface/simple/simpleinterface.h"
#include "src/ui/interface/advanced/advancedinterface.h"
#include "src/ui/interface/study/studyinterface.h"
#include "src/ui/dialog/settingsdialog.h"


#include "src/core/dbghelper.h"
#include "src/core/moduletools.h"
#include "src/ui/context.h"
#include "config.h"

MainWindow::MainWindow(Context *parent) :
    QMainWindow(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_reloadLang = false;
    m_context = parent;
}

MainWindow::~MainWindow()
{
    DEBUG_FUNC_NAME;
    delete ui;
    ui = nullptr;
}

void MainWindow::init(bool firstStart)
{
    loadInterface();
    restoreSession();

    if(firstStart) {
        QTimer::singleShot(1, this, SLOT(showSettingsDialog_Module()));
    }
    connect(m_actions, SIGNAL(_setFullScreen(bool)), this, SLOT(fullscreen(bool)));

}
void MainWindow::loadInterface()
{
    DEBUG_FUNC_NAME
    const QString interface = m_settings->session.getData("interface", QString("advanced")).toString();

    if(interface == "advanced") {
        loadAdvancedInterface();
    } else if(interface == "simple") {
        loadSimpleInterface();
    } else {
        myWarning() << "this interface" << interface << "does not exist";
    }
}
void MainWindow::deleteInterface()
{
    DEBUG_FUNC_NAME
    if(m_interface->hasToolBar()) {
        foreach(QToolBar * bar, m_toolBarList) {
            removeToolBar(bar);
        }
    }

    if(m_interface->hasMenuBar()) {
        delete m_menuBar;
    }
    QHash<DockWidget*, Qt::DockWidgetArea> docks = m_interface->docks();
    QHashIterator<DockWidget*, Qt::DockWidgetArea> it(docks);
    while(it.hasNext()) {
        it.next();
        if(it.key() != nullptr) {
            removeDockWidget(it.key());
        }
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
}

void MainWindow::loadAdvancedInterface()
{
    DEBUG_FUNC_NAME
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

void MainWindow::saveSettings(Settings newSettings, bool modifedModuleSettings)
{
    Settings oldSettings = *m_settings;

    *m_settings = newSettings;
    m_context->writeSettings();

    if(oldSettings.language != newSettings.language) {
        loadLanguage(newSettings.language);
    }

    if(m_interface->name() != newSettings.session.getData("interface")) {
        reloadInterface();
    }
    emit settingsChanged(oldSettings, newSettings, modifedModuleSettings);
}
void MainWindow::showSettingsDialog(int tabID)
{
    QPointer<SettingsDialog> dlg = new SettingsDialog(this);
    connect(dlg, SIGNAL(settingsChanged(Settings, bool)), this, SLOT(saveSettings(Settings, bool)));
    dlg->setSettings(*m_settings);
    dlg->setWindowTitle(tr("Configuration"));
    dlg->setCurrentTab(tabID);
    dlg->exec();
    delete dlg;
}
void MainWindow::showSettingsDialog_Module()
{
    showSettingsDialog(0);
}
void MainWindow::showSettingsDialog_General()
{
    showSettingsDialog(2);
}
void MainWindow::setTranslator(QTranslator *my, QTranslator *qt)
{
    myappTranslator = my;
    qtTranslator = qt;
}
void MainWindow::loadLanguage(QString language)
{
    QStringList avLang;
    avLang << QString(_AV_LANG).split(";");
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
            QMessageBox::warning(this, tr("Installing language failed."), tr("Please choose another language."));
    }

    qtTranslator->load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

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
void MainWindow::fullscreen(bool enable)
{
    DEBUG_FUNC_NAME
    if(enable) {
       // this->menuBar()->setHidden(true);
    } else {
      //  menuBar()->setHidden(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    m_settings->session.setData("id", m_settings->sessionID);
    m_settings->session.setData("mainWindowGeometry", QVariant(saveGeometry()));
    m_settings->session.setData("mainWindowState", QVariant(saveState()));
    emit closing();
    m_context->writeSettings();
}
void MainWindow::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        //myDebug() << "retranslate";
        ui->retranslateUi(this);
        if(m_reloadLang) {
            if(m_interface->hasMenuBar()) {
                m_menuBar = m_interface->menuBar();
                setMenuBar(m_menuBar);
            }
            m_reloadLang = false;
        }
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(e);
}
