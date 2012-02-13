/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#include "webform.h"
#include "ui_webform.h"
#include <QtGui/QToolBar>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QPrintPreviewDialog>
#include <QtWebKit/QWebHistory>
#include <QtCore/QPointer>

WebForm::WebForm(QWidget *parent) :
    WebViewForm(parent),
    m_ui(new Ui::WebForm)
{
    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(m_view);
    connect(m_ui->lineEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
    connect(m_view, SIGNAL(urlChanged(QUrl)), this, SLOT(adjustLocation()));
    connect(m_view, SIGNAL(loadStarted()), this, SLOT(loadStarted()));

    m_ui->toolButton_back->setIcon(m_view->pageAction(QWebPage::Back)->icon());
    m_ui->toolButton_back->setToolTip(m_view->pageAction(QWebPage::Back)->toolTip());

    m_ui->toolButton_forward->setIcon(m_view->pageAction(QWebPage::Forward)->icon());
    m_ui->toolButton_forward->setToolTip(m_view->pageAction(QWebPage::Forward)->toolTip());

    m_ui->toolButton_reloadStop->setIcon(m_view->pageAction(QWebPage::Reload)->icon());
    m_ui->toolButton_reloadStop->setToolTip(m_view->pageAction(QWebPage::Reload)->toolTip());

    connect(m_ui->toolButton_forward, SIGNAL(clicked()), m_view, SLOT(forward()));
    connect(m_ui->toolButton_back, SIGNAL(clicked()), m_view, SLOT(back()));
    connect(m_ui->toolButton_reloadStop, SIGNAL(clicked()), m_view, SLOT(reload()));

    m_ui->toolButton_back->setShortcut(QKeySequence::Back);
    m_ui->toolButton_forward->setShortcut(QKeySequence::Forward);

    m_page = NULL;
}

WebForm::~WebForm()
{
    delete m_ui;
    if(m_page != NULL) {
        delete m_page;
        m_page = NULL;
    }
}

Form::FormType WebForm::type() const
{
    return Form::WebForm;
}

// shamelessly copied from Qt Demo Browser
QUrl WebForm::guessUrlFromString(const QString &string)
{
    QString urlStr = string.trimmed();
    QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));

    // Check if it looks like a qualified URL. Try parsing it and see.
    bool hasSchema = test.exactMatch(urlStr);
    if(hasSchema) {
        QUrl url(urlStr, QUrl::TolerantMode);
        if(url.isValid())
            return url;
    }

    // Might be a file.
    if(QFile::exists(urlStr))
        return QUrl::fromLocalFile(urlStr);

    // Might be a shorturl - try to detect the schema.
    if(!hasSchema) {
        int dotIndex = urlStr.indexOf(QLatin1Char('.'));
        if(dotIndex != -1) {
            QString prefix = urlStr.left(dotIndex).toLower();
            QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
            QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
            if(url.isValid())
                return url;
        }
    }

    // Fall back to QUrl's own tolerant parser.
    return QUrl(string, QUrl::TolerantMode);
}

void WebForm::pharseUrl(QString url)
{
    url = url.remove(0, ModuleTools::webPageScheme.size());
    openModule(url.toInt());
}
void WebForm::pharseWebUrl(const QString &url)
{
    m_view->load(QUrl(url));
}
void WebForm::openModule(const int moduleID)
{
    DEBUG_FUNC_NAME;
    if(m_page == NULL) {
        m_page = new WebPage();
        //m_moduleManager->initSimpleModule(m_page);
        m_page->setSettings(m_settings);
        m_page->setModuleMap(m_moduleManager->m_moduleMap);
        m_page->setModuleID(moduleID);

        QUrl url = m_page->getUrl();
        m_view->load(url);
    } else {
        m_page->setModuleID(moduleID);

        QUrl url = m_page->getUrl();
        m_view->load(url);
    }
}

void WebForm::init()
{
}

void WebForm::changeLocation()
{
    m_view->load(guessUrlFromString(m_ui->lineEdit->text()));
}

void WebForm::adjustLocation()
{
    m_ui->lineEdit->setText(m_view->url().toString());
}


void WebForm::activated()
{
    m_actions->clearBooks();
    m_actions->clearChapters();
    if(m_page != NULL) {
        m_actions->setTitle(m_page->moduleTitle());
        m_actions->setCurrentModule(m_page->moduleID());
    }
}

void WebForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "web");
    m_settings->session.file()->setValue(a + "url", m_view->url());

    QByteArray history;
    QDataStream historyStream(&history, QIODevice::ReadWrite);
    QWebHistory *hist  = m_view->history();
    historyStream << *(hist);
    m_settings->session.file()->setValue(a + "history", history.toBase64());
}

void WebForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";

    QUrl url = m_settings->session.file()->value(a + "url").toUrl();
    if(url.isValid())
        m_view->load(url);

    QByteArray history = QByteArray::fromBase64(m_settings->session.file()->value(a + "history").toByteArray());
    QDataStream readingStream(&history, QIODevice::ReadOnly);
    QWebHistory *hist  = m_view->history();
    readingStream >> *(hist);
}

void WebForm::loadStarted()
{
    if(m_view->history()->canGoBack()) {
        m_ui->toolButton_back->setDisabled(false);
    } else {
        m_ui->toolButton_back->setDisabled(true);
    }

    if(m_view->history()->canGoForward()) {
        m_ui->toolButton_forward->setDisabled(false);
    } else {
        m_ui->toolButton_forward->setDisabled(true);
    }

}
