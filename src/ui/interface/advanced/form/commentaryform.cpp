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
    if(!url.fromUrl(string)) {
        return;
    }
    pharseUrl(url);
}
void CommentaryForm::pharseUrl(VerseUrl url)
{
    if(m_com == NULL) {
        m_com = new WebCommentary();
        m_moduleManager->initSimpleModule(m_com);
    }
    if(m_com == NULL) {
        myDebug() << "its null";
        return;
    }
    bool showStart = false;

    if(m_url.isValid()) {
        m_url = m_url.applyUrl(url);
        showStart = true;
    } else {
        m_url = url;
       /* if(loaded() && !m_url.hasModuleID()) {
            m_url.setModuleID(verseModule()->moduleID());
        }*/
    }
    Ranges ranges;
    foreach(VerseUrlRange range, m_url.ranges()) {
        ranges.addRange(range.toRange());
    }
    ranges.setSource(m_url);
    m_com->setModuleID(ranges.getList().first().moduleID());
    Response *res = m_com->readRanges(ranges);
    myDebug() << res;
    if(res != NULL && res->isValid() && res->type() == Response::UrlReponse) {
        UrlResponse *u = (UrlResponse*) res;
        myDebug() << u->url();
        m_view->load(QUrl(u->url()));
    }

    RefText ref;
    ref.setSettings(m_settings);
    ui->lineEdit->setText(ref.toString(ranges));
}
void CommentaryForm::changeLocation()
{
    BibleLink link(m_com->moduleID(), m_com->versification());
    pharseUrl(link.getUrl(ui->lineEdit->text()));
}

void CommentaryForm::init()
{
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

void CommentaryForm::save()
{
   /* const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "web");
    m_settings->session.file()->setValue(a + "url", m_view->url());

    QByteArray history;
    QDataStream historyStream(&history, QIODevice::ReadWrite);
    QWebHistory *hist  = m_view->history();
    historyStream << *(hist);
    m_settings->session.file()->setValue(a + "history", history.toBase64());*/
}

void CommentaryForm::restore(const QString &key)
{
    /*const QString a = m_settings->session.id() + "/windows/" + key + "/";

    QUrl url = m_settings->session.file()->value(a + "url").toUrl();
    if(url.isValid())
        m_view->load(url);

    QByteArray history = QByteArray::fromBase64(m_settings->session.file()->value(a + "history").toByteArray());
    QDataStream readingStream(&history, QIODevice::ReadOnly);
    QWebHistory *hist  = m_view->history();
    readingStream >> *(hist);*/
}
