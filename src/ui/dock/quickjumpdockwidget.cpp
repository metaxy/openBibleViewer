/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "src/core/biblelink.h"
#include "src/core/dbghelper.h"
#include <QtGui/QKeyEvent>

QuickJumpDockWidget::QuickJumpDockWidget(QWidget *parent) :
    DockWidget(parent),
    ui(new Ui::QuickJumpDockWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_goTo, SIGNAL(clicked()), this, SLOT(goToPos()));
    connect(ui->lineEdit_goTo, SIGNAL(returnPressed()), this, SLOT(goToPos()));

    m_completer = NULL;
}

QuickJumpDockWidget::~QuickJumpDockWidget()
{
    delete ui;
}
void QuickJumpDockWidget::init()
{
    DEBUG_FUNC_NAME;
    connect(m_actions, SIGNAL(_updateBooks(Versification*)), this, SLOT(setBooks(Versification*)));
}
void QuickJumpDockWidget::setBooks(Versification *v11n)
{
    //DEBUG_FUNC_NAME;
    m_books = v11n->bookNames().values();

    QStringList l;
    l << m_books;
    l << m_hist;
    /*if(m_completer != NULL) {
        delete m_completer;
        m_completer = NULL;
    }*/
    m_completer = new QCompleter(l, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_goTo->setCompleter(m_completer);
}

void QuickJumpDockWidget::goToPos()
{
    DEBUG_FUNC_NAME;
    const QString text = ui->lineEdit_goTo->text();
    if(text.isEmpty() || !m_moduleManager->bibleLoaded()) {
        myWarning() << "no module loaded";
        return;
    }
    m_hist << text;

    QStringList l;
    l << m_books;
    l << m_hist;
    /*if(m_completer != NULL) {
        delete m_completer;
        m_completer = NULL;
    }*/
    m_completer = new QCompleter(l, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_goTo->setCompleter(m_completer);

    BibleLink link(m_moduleManager->verseModule()->moduleID(), m_moduleManager->verseModule()->versification());

    m_actions->get(link.getUrl(text));
    return;
}
void QuickJumpDockWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch(e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
