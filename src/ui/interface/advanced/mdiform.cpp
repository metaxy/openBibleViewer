/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "mdiform.h"
#include "ui_mdiform.h"
#include "src/core/dbghelper.h"
#include "biblelistwidget.h"
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QCursor>
#include "src/core/core.h"
MdiForm::MdiForm(QWidget *parent) : QWidget(parent), m_ui(new Ui::MdiForm)
{
    m_ui->setupUi(this);

    m_view = new WebView(this);
    m_view->setObjectName("webView");
    m_view->setUrl(QUrl("about:blank"));
    m_ui->verticalLayout->addWidget(m_view);

    m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(m_ui->toolButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
    connect(m_ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));
    connect(m_ui->toolButton_bibleList,SIGNAL(clicked()), this, SLOT(showBibleListMenu()));

    setButtons();
    //m_ui->textBrowser->installEventFilter(this);

}
void MdiForm::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void MdiForm::historyGetUrl(QString url)
{
    browserHistory.setCurrent(url);
    setButtons();
}
void MdiForm::backward()
{
    emit historyGo(browserHistory.backward());
    setButtons();
}
void MdiForm::forward()
{
    emit historyGo(browserHistory.forward());
    setButtons();
}
void MdiForm::setButtons()
{
    if (browserHistory.backwardAvailable()) {
        m_ui->toolButton_backward->setDisabled(false);
    } else {
        m_ui->toolButton_backward->setDisabled(true);
    }
    if (browserHistory.forwardAvailable()) {
        m_ui->toolButton_forward->setDisabled(false);
    } else {
        m_ui->toolButton_forward->setDisabled(true);
    }
}

void MdiForm::showBibleListMenu()
{
    BibleListWidget *w = new BibleListWidget;
    w->setModuleManager(m_moduleManager);
    w->setSettings(m_settings);
    w->setBibleDisplay(m_bibleDisplay);
/*   QPoint p = QCursor::pos();
    p.setX(p.x()-w->width());
    w->move(p);*/
    w->init();
    w->exec();
}

bool MdiForm::eventFilter(QObject *obj, QEvent *event)
{
    /*if (obj == m_view) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Left) {
                emit previousChapter();
                return true;
            } else if (keyEvent->key() == Qt::Key_Right) {
                emit nextChapter();
                return true;
            } else {
                return QWidget::eventFilter(obj, event);
            }

        } else {
            return QWidget::eventFilter(obj, event);;
        }
    } else {
        return QWidget::eventFilter(obj, event);
    }*/
    return QWidget::eventFilter(obj, event);
}
MdiForm::~MdiForm()
{
    delete m_ui;
}
