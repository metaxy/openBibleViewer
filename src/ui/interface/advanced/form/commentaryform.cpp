#include "commentaryform.h"
#include "ui_commentaryform.h"

CommentaryForm::CommentaryForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::CommentaryForm)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(m_view);
    m_com = NULL;
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
void CommentaryForm::pharseUrl(QString url)
{
    url = url.remove(0, ModuleTools::webPageScheme.size());
    openModule(url.toInt());
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
