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
#include "commentaryform.h"
#include "ui_commentaryform.h"
#include "src/module/response/urlresponse.h"
#include "src/core/verse/reftext.h"
#include "src/core/link/biblelink.h"
CommentaryForm::CommentaryForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::CommentaryForm)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_view);
    m_com = NULL;
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(changeLocation()));

    connect(ui->toolButton_backward, SIGNAL(clicked()), this, SLOT(backward()));
    connect(ui->toolButton_forward, SIGNAL(clicked()), this, SLOT(forward()));

    ui->toolButton_backward->setShortcut(QKeySequence::Back);
    ui->toolButton_forward->setShortcut(QKeySequence::Forward);
    setButtons();
}

void CommentaryForm::init()
{
    m_com = new WebCommentary();
    m_moduleManager->initSimpleModule(m_com);
}
CommentaryForm::~CommentaryForm()
{
    delete ui;
    if(m_com != NULL) {
        delete m_com;
        m_com = NULL;
    }
}
Form::FormType CommentaryForm::type() const
{
    return Form::CommentaryForm;
}
void CommentaryForm::pharseUrl(QString string)
{
    VerseUrl url;
    if(!url.fromStringUrl(string)) {
        myWarning() << "pharsing url failed:" << string;
        return;
    }
    pharseUrl(url);
}
void CommentaryForm::pharseUrl(VerseUrl url)
{
    if(m_com == NULL) {
        myWarning() << "m_com is null";
        return;
    }
    bool showStart = false;

    if(m_url.isValid()) {
        m_url = m_url.applyUrl(url);
        showStart = true;
    } else {
        m_url = url;
        if(loaded() && !m_url.hasModuleID()) {
            m_url.setModuleID(m_com->moduleID());
        }
    }
    Ranges ranges;
    foreach(VerseUrlRange range, m_url.ranges()) {
        ranges.addRange(range.toRange());
    }
    ranges.setSource(m_url);
    showRanges(ranges, m_url);

}
void CommentaryForm::showRanges(Ranges ranges, const VerseUrl &source)
{
    if(!m_com) {
        myWarning() << "m_com is null";
        return;
    }
    m_com->setModuleID(ranges.getList().first().moduleID());
    Response *res = m_com->readRanges(ranges);

    if(res != NULL && res->isValid() && res->type() == Response::UrlReponse) {
        UrlResponse *u = (UrlResponse*) res;
        m_view->load(QUrl(u->url()));
    }

    RefText ref;
    ref.setSettings(m_settings);
    ui->lineEdit->setText(ref.toString(ranges));

    historySetUrl(source.toString());
}

void CommentaryForm::changeLocation()
{
    BibleLink link(m_com->moduleID(), m_com->versification());
    pharseUrl(link.getUrl(ui->lineEdit->text()));
}

void CommentaryForm::activated()
{
    m_actions->clearBooks();
    m_actions->clearChapters();
    /*if(m_page != NULL) {
        m_actions->setTitle(m_page->moduleTitle());
        m_actions->setCurrentModule(m_page->moduleID());
    }*/
}
bool CommentaryForm::loaded()
{
    return m_com != NULL && m_com->loaded();
}

void CommentaryForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "commentary");
   /* m_settings->session.file()->setValue(a + "url", m_view->url());

    QByteArray history;
    QDataStream historyStream(&history, QIODevice::ReadWrite);
    QWebHistory *hist  = m_view->history();
    historyStream << *(hist);
    m_settings->session.file()->setValue(a + "history", history.toBase64());*/

    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom", m_view->zoomFactor());

    m_settings->session.file()->setValue(a + "hist1", m_browserHistory.data1());
    m_settings->session.file()->setValue(a + "hist2", m_browserHistory.data2());
    m_settings->session.file()->setValue(a + "hist3", m_browserHistory.data3());

}

void CommentaryForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrollPosition").toPoint();

    const QVariant v = m_settings->session.file()->value(a + "hist1");
    const QVariant v2 = m_settings->session.file()->value(a + "hist2");
    if(!v.toStringList().isEmpty() || !v2.toStringList().isEmpty()) {
        m_browserHistory.setData1(v);
        m_browserHistory.setData2(v2);
        m_browserHistory.setData3(m_settings->session.file()->value(a + "hist3"));
        setButtons();
    }


    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);

   /* QUrl url = m_settings->session.file()->value(a + "url").toUrl();
    if(url.isValid())
        m_view->load(url);

   QByteArray history = QByteArray::fromBase64(m_settings->session.file()->value(a + "history").toByteArray());
    QDataStream readingStream(&history, QIODevice::ReadOnly);
    QWebHistory *hist  = m_view->history();
    readingStream >> *(hist);*/
}
void CommentaryForm::historySetUrl(QString url)
{
    m_browserHistory.setCurrent(url);
    setButtons();
}
void CommentaryForm::backward()
{
    m_actions->get(m_browserHistory.backward());
    setButtons();
}
void CommentaryForm::forward()
{
    m_actions->get(m_browserHistory.forward());
    setButtons();
}
void CommentaryForm::setButtons()
{
    if(m_browserHistory.backwardAvailable()) {
        ui->toolButton_backward->setDisabled(false);
    } else {
        ui->toolButton_backward->setDisabled(true);
    }
    if(m_browserHistory.forwardAvailable()) {
        ui->toolButton_forward->setDisabled(false);
    } else {
        ui->toolButton_forward->setDisabled(true);
    }
}
