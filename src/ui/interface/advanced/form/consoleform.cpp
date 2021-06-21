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
#include "consoleform.h"
#include "ui_consoleform.h"
#include "src/api/moduleapi.h"
#include "src/api/notesapi.h"
#include "src/api/searchapi.h"
#include "src/api/api.h"
ConsoleForm::ConsoleForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::ConsoleForm)
{
    ui->setupUi(this);
}

ConsoleForm::~ConsoleForm()
{
    delete ui;
}

void ConsoleForm::init()
{
    attach();
    //TODO: WEB connect(m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attach()));
    m_view->load(QUrl::fromLocalFile(":/data/html/consoleform.html"));

    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));
}

ModuleID ConsoleForm::moduleID()
{
    return ModuleIDNotSet;
}

void ConsoleForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrollPosition").toPoint();
    m_view->scrollTo(scroll);
    m_view->setZoomFactor(zoom);
}

void ConsoleForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "console");
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
}

Form::FormType ConsoleForm::type() const
{
    return Form::ConsoleForm;
}

void ConsoleForm::attach()
{
    addJS(":/data/js/tools.js");
    addJS(":/data/js/jquery-1.4.2.min.js");
    addJS(":/data/js/underscore-min.js");

    addWebChannel(m_api->searchApi());
    addWebChannel(m_api->moduleApi());
    addWebChannel(m_api->notesApi());
    addWebChannel(m_actions);
}
void ConsoleForm::showContextMenu(QContextMenuEvent* ev)
{
    //DEBUG_FUNC_NAME
    QScopedPointer<QMenu> contextMenu(new QMenu(this));
    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), this);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    if(m_view->hasSelection()) {
        actionCopy->setEnabled(true);
    } else {
        actionCopy->setEnabled(false);
    }
    contextMenu->exec(ev->globalPos());

}
