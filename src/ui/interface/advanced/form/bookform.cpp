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
#include "bookform.h"
#include "ui_bookform.h"
#include "src/core/module/response/stringresponse.h"
BookForm::BookForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::BookForm)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_view);
    m_book = NULL;
}

BookForm::~BookForm()
{
    if(m_book != NULL) {
        delete m_book;
        m_book = NULL;
    }
    delete ui;
}
void BookForm::init()
{

}

void BookForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrool").toPoint();
    const QString uid = m_settings->session.file()->value(a + "uid").toString();

    int moduleID = m_moduleManager->m_moduleMap->UIDtoID(uid);
    if(moduleID != -1) {
        loadModule(moduleID);
        show();
    }

    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);
}

void BookForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "book");
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
    if(m_book != NULL) {
        m_settings->session.file()->setValue(a + "uid", m_moduleManager->getModule(m_book->moduleID())->moduleUID());
    }
}

Form::FormType BookForm::type() const
{
    return Form::BookForm;
}

void BookForm::activated()
{
    m_parentSubWindow->update();
    m_view->update();
    if(m_book == NULL)
        return;
    m_actions->setTitle(m_book->moduleTitle());
    m_actions->setCurrentModule(m_book->moduleID());
}
void BookForm::loadModule(const int moduleID)
{
    if(m_book == NULL) {
        m_book = new Book();
        m_moduleManager->initSimpleModule(m_book);
    } else {
        m_book->clearData();
    }
    ModuleTools::ModuleType type = m_moduleManager->getModule(moduleID)->moduleType();
    m_book->setModuleType(type);
    m_book->setModuleID(moduleID);

    m_actions->setTitle(m_book->moduleTitle());
    m_actions->setCurrentModule(m_book->moduleID());

}

void BookForm::show()
{
    if(!m_book) {
        m_view->setHtml(tr("Not loaded"));
        return;
    }

    StringResponse *r = (StringResponse *)m_book->getAll();
    if(r) {
        {
            QString cssFile;
            if(m_book)
                cssFile = m_settings->getModuleSettings(m_book->moduleID())->styleSheet;

            if(cssFile.isEmpty())
                cssFile = ":/data/css/default.css";

            m_view->settings()->setUserStyleSheetUrl(QUrl::fromLocalFile(cssFile));
        }
        m_view->setHtml(r->data());
    }
    delete r;
}
