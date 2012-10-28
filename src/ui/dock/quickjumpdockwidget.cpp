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
#include "quickjumpdockwidget.h"
#include "ui_quickjumpdockwidget.h"
#include "src/core/link/biblelink.h"
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

void QuickJumpDockWidget::setWindowManager(WindowManager *manager)
{
    m_windowManager = manager;
}

void QuickJumpDockWidget::init()
{
    connect(m_actions, SIGNAL(_updateBooks(QSharedPointer<Versification>)), this, SLOT(setBooks(QSharedPointer<Versification>)));
}

void QuickJumpDockWidget::setBooks(QSharedPointer<Versification> v11n)
{
    //DEBUG_FUNC_NAME;
    m_books = v11n->bookNames().values();

    QStringList l;
    l << m_books;
    l << m_hist;
    if(ui->lineEdit_goTo->completer() != NULL) {
        delete ui->lineEdit_goTo->completer();
        m_completer = NULL;
    }
    m_completer = new QCompleter(l, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_goTo->setCompleter(m_completer);
}

void QuickJumpDockWidget::goToPos()
{
    DEBUG_FUNC_NAME;
    const QString text = ui->lineEdit_goTo->text();
    if(text.isEmpty()) {
        myWarning() << "no module loaded";
        return;
    }
    m_hist << text;

    QStringList l;
    l << m_books;
    l << m_hist;
    if(ui->lineEdit_goTo->completer()!= NULL) {
        delete ui->lineEdit_goTo->completer();
        m_completer = NULL;
    }
    m_completer = new QCompleter(l, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_goTo->setCompleter(m_completer);
    if(m_windowManager->activeForm() && m_windowManager->activeForm()->type() == Form::BibleForm) {
        BibleForm *f = (BibleForm*)(m_windowManager->activeForm());
        BibleLink link(f->verseModule()->moduleID(), f->verseModule()->versification());

        m_actions->get(link.getUrl(text));
    } else {

        m_windowManager->newSubWindow(Form::BibleForm);
        int defaultModuleID = -1;
        QMapIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
        while(i.hasNext()) {
            i.next();
            if(i.value()->defaultModule == ModuleTools::DefaultBibleModule)
                defaultModuleID = i.key();

        }
        if(defaultModuleID == -1) {
            QMapIterator<int, Module*> i2 = m_moduleManager->m_moduleMap->it();
            while(i2.hasNext()) {
                i2.next();
                if(i2.value()->moduleClass() == ModuleTools::BibleModuleClass)
                    defaultModuleID = i2.key();
            }
        }
        BibleLink link(defaultModuleID, m_settings->getV11N(defaultModuleID));
        if(link.isBibleLink(text)) {
            m_actions->get(link.getUrl(text));
        }

    }
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
