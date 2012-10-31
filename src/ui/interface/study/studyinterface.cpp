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
#include "studyinterface.h"
#include "ui_studyinterface.h"

StudyInterface::StudyInterface(QWidget *parent) :
    Interface(parent),
    ui(new Ui::StudyInterface)
{
    ui->setupUi(this);
}
void StudyInterface::init()
{
}

StudyInterface::~StudyInterface()
{
    delete ui;
}

void StudyInterface::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
bool StudyInterface::hasMenuBar()
{
    return false;
}

QMenuBar* StudyInterface::menuBar()
{
    return nullptr;
}

bool StudyInterface::hasToolBar()
{
    return false;
}

QList<QToolBar*> StudyInterface::toolBars()
{
    return QList<QToolBar*>();
}

QHash<DockWidget *, Qt::DockWidgetArea> StudyInterface::docks()
{
    return QHash<DockWidget *, Qt::DockWidgetArea> ();
}

void StudyInterface::createDocks()
{

}

void StudyInterface::createToolBars()
{

}

void StudyInterface::createMenu()
{

}
QString StudyInterface::name() const
{
    return "study";
}
