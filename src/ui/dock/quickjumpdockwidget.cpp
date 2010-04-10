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
#include "quickjumpdockwidget.h"
#include "ui_quickjumpdockwidget.h"
#include "src/core/goto.h"
#include "src/core/dbghelper.h"
#include <QtGui/QKeyEvent>

QuickJumpDockWidget::QuickJumpDockWidget(QWidget *parent) :
        DockWidget(parent),
        ui(new Ui::QuickJumpDockWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_goTo, SIGNAL(clicked()), this, SLOT(goToPos()));
    connect(ui->lineEdit_goTo, SIGNAL(returnPressed()), this, SLOT(goToPos()));
}

QuickJumpDockWidget::~QuickJumpDockWidget()
{
    delete ui;
}
void QuickJumpDockWidget::init()
{
}
void QuickJumpDockWidget::setBooks(QStringList list)
{
    books = list;

    QStringList l;
    l << books;
    l << hist;
    completer = new QCompleter(l, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_goTo->setCompleter(completer);
}

void QuickJumpDockWidget::goToPos()
{
    QString text = ui->lineEdit_goTo->text();
    hist << text;

    QStringList l;
    l << books;
    l << hist;
    completer = new QCompleter(l, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_goTo->setCompleter(completer);

    GoTo go(m_moduleManager->m_bible.bibleID(), m_moduleManager->m_bible.bookFullName);
    QString url = go.getUrl(text);
    emit get(url);
    return;
}
void QuickJumpDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
