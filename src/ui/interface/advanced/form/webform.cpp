#include "webform.h"
#include "ui_webform.h"
#include <QtGui/QToolBar>
WebForm::WebForm(QWidget *parent) :
    Form(parent),
    m_ui(new Ui::WebForm)
{
    m_ui->setupUi(this);

    connect(m_ui->lineEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
    connect(m_ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(adjustLocation()));

    m_ui->toolButton_back->setIcon(m_ui->webView->pageAction(QWebPage::Back)->icon());
    m_ui->toolButton_back->setToolTip(m_ui->webView->pageAction(QWebPage::Back)->toolTip());
    connect(m_ui->toolButton_back, SIGNAL(clicked()), m_ui->webView, SLOT(back()));

    m_ui->toolButton_forward->setIcon(m_ui->webView->pageAction(QWebPage::Forward)->icon());
    m_ui->toolButton_forward->setToolTip(m_ui->webView->pageAction(QWebPage::Forward)->toolTip());
    connect(m_ui->toolButton_forward, SIGNAL(clicked()), m_ui->webView, SLOT(forward()));

    /*QToolBar *toolBar = new QToolBar(tr("Navigation"), this);
    toolBar->addAction();
    toolBar->addAction(m_ui->webView->pageAction(QWebPage::Forward));
    toolBar->addAction(m_ui->webView->pageAction(QWebPage::Reload));
    toolBar->addAction(m_ui->webView->pageAction(QWebPage::Stop));*/


}

WebForm::~WebForm()
{
    delete m_ui;
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
void WebForm::init()
{
}
void WebForm::changeLocation()
{
    m_ui->webView->load(guessUrlFromString(m_ui->lineEdit->text()));
}
void WebForm::adjustLocation()
{
    m_ui->lineEdit->setText(m_ui->webView->url().toString());
}

void WebForm::copy()
{
    m_ui->webView->triggerPageAction(QWebPage::Copy);
}

void WebForm::selectAll()
{
    m_ui->webView->triggerPageAction(QWebPage::SelectAll);
}

void WebForm::print()
{

}

void WebForm::printPreview()
{

}

void WebForm::saveFile()
{

}

QString WebForm::selectedText()
{
    return m_ui->webView->selectedText();
}

void WebForm::zoomIn()
{
    m_ui->webView->setZoomFactor(m_ui->webView->zoomFactor() + 0.1);
}

void WebForm::zoomOut()
{
    m_ui->webView->setZoomFactor(m_ui->webView->zoomFactor() - 0.1);
}

void WebForm::activated()
{
    m_actions->clearBooks();
    m_actions->clearChapters();
}
