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
#include "webviewform.h"
#include <QPointer>
#include <QPrinter>
#include <QFileInfo>
#include <QTextStream>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QFileDialog>
#include <QTextDocumentWriter>

#include <QClipboard>
#include "src/ui/dialog/moduleselectdialog.h"
#include "src/api/api.h"
WebViewForm::WebViewForm(QWidget *parent) :
    Form(parent)
{
    m_view = new WebView(this);
    m_view->setObjectName("webView");
    m_view->setUrl(QUrl("about:blank"));
    m_view->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    m_view->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    m_view->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

    m_view->setLayoutDirection(Qt::LayoutDirectionAuto);
}
WebViewForm::~WebViewForm()
{
}

void WebViewForm::copy()
{
    m_view->page()->triggerAction(QWebEnginePage::Copy);
}

void WebViewForm::selectAll()
{
    m_view->page()->triggerAction(QWebEnginePage::SelectAll);
}

void WebViewForm::print()
{
    //TODO: WEB
   /* QSharedPointer<QPrinter> printer(new QPrinter());
    QSharedPointer<QPrintDialog> dialog(new QPrintDialog(printer.data(), this));
    dialog->setWindowTitle(tr("Print"));
    if(dialog->exec() == QDialog::Accepted) {
        m_view->page()->print(&printer, [&dialog, &printer](bool success){dialog.clear(); printer.clear();});
    }*/
}

void WebViewForm::printPreview()
{
    QPrinter printer;
    QPointer<QPrintPreviewDialog> preview = new QPrintPreviewDialog(&printer, this);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), m_view, SLOT(print(QPrinter *)));
    preview->exec();
    delete preview;
}

void WebViewForm::saveFile()
{
    const QString lastPlace = m_settings->session.getData("lastSaveFilePlace").toString();
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save output"), lastPlace, tr("Open Document (*.odt);;PDF (*.pdf);;Html (*.html *.htm);;Plain (*.txt)"));
    QFileInfo fi(fileName);
    m_settings->session.setData("lastSaveFilePlace", fi.path());
    if(fi.suffix().compare("html", Qt::CaseInsensitive) == 0 ||
            fi.suffix().compare("htm", Qt::CaseInsensitive) == 0) {
        m_view->page()->toHtml([&fileName](const QString &html){
           QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            QTextStream out(&file);
            out << html;
            file.close();
        }); 
        
    } else if(fi.suffix().compare("pdf", Qt::CaseInsensitive) == 0) {
        m_view->page()->printToPdf(fileName);
    } else if(fi.suffix().compare("odt", Qt::CaseInsensitive) == 0) {
        m_view->page()->toHtml([&fileName](const QString &html){
            QTextDocumentWriter writer;
            writer.setFormat("odf");
            writer.setFileName(fileName);
            QTextDocument doc;
            doc.setHtml(html);
            writer.write(&doc);
        }); 
    } else {
        m_view->page()->toPlainText([&fileName](const QString &text){
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            QTextStream out(&file);
            out << text;
            file.close();
        }); 
    }
}

QString WebViewForm::selectedText()
{
    return m_view->selectedText();
}

void WebViewForm::zoomIn()
{
    m_view->setZoomFactor(m_view->zoomFactor() + 0.1);
}

void WebViewForm::zoomOut()
{
    m_view->setZoomFactor(m_view->zoomFactor() - 0.1);
}

void WebViewForm::openInNewTab()
{
    m_actions->get(m_contextMenuUrl, Actions::OpenInNewWindow);
}

void WebViewForm::openHere()
{
    m_actions->get(m_contextMenuUrl, Actions::NoModifer);
}

void WebViewForm::copyLinkText()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_contextMenuText);
}

void WebViewForm::copyLinkUrl()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_contextMenuUrl);
}
void WebViewForm::openIn()
{
    QAction *s = (QAction*) sender();
    if(s != NULL) {
        int moduleID = s->data().toInt();
        openInM(moduleID);
    }
}
void WebViewForm::openInNew()
{
    QAction *s = (QAction*) sender();
    if(s != NULL) {
        int moduleID = s->data().toInt();
        openInNewM(moduleID);
    }
}
void WebViewForm::openIn(QString url, const int moduleID, const Actions::OpenLinkModifiers mod)
{
    DEBUG_FUNC_NAME;
    myDebug() << url << m_settings->getModuleSettings(moduleID)->moduleName;

    if(url.startsWith(ModuleTools::strongScheme)) {
        url = url.remove(0, ModuleTools::strongScheme.size());
        m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url, mod);
    } else if(url.startsWith(ModuleTools::rmacScheme)) {
        url = url.remove(0, ModuleTools::rmacScheme.size());
        m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url, mod);
    } else if(url.startsWith(ModuleTools::dictScheme)) {
        url = url.remove(0, ModuleTools::dictScheme.size());
        m_actions->get(ModuleTools::dictScheme + QString::number(moduleID) + "/" + url, mod);
    }  else if(url.startsWith(ModuleTools::verseScheme)) {
        VerseUrl vurl;
        vurl.fromStringUrl(url);
        vurl.setModuleID(moduleID);
        m_actions->get(vurl, mod);
    } else {
        m_actions->get(url, mod);
    }
    m_settings->getModuleSettings(moduleID)->stats_timesOpened++;
}
void WebViewForm::openInM(const int moduleID)
{
    openIn(m_contextMenuUrl, moduleID, Actions::NoModifer);
}
void WebViewForm::openInNewM(const int moduleID)
{
    openIn(m_contextMenuUrl, moduleID, Actions::OpenInNewWindow);
}
void WebViewForm::openInS()
{
    QPointer<ModuleSelectDialog> dialog = new ModuleSelectDialog(this);
    connect(dialog.data(), SIGNAL(moduleSelected(int)), this, SLOT(openInM(int)));
    dialog->exec();
    delete dialog;
}

void WebViewForm::openInNewS()
{
    QPointer<ModuleSelectDialog> dialog = new ModuleSelectDialog(this);
    connect(dialog.data(), SIGNAL(moduleSelected(int)), this, SLOT(openInNewM(int)));
    dialog->exec();
    delete dialog;
}
bool WebViewForm::showResponse(Response *res)
{
    m_lastUrl.clear();
    if(res != NULL && res->isValid()) {
        if(res->type() == Response::StringReponse) {
            showStringResponse((StringResponse*) res);
        } else if(res->type() == Response::UrlReponse) {
            showUrlResponse((UrlResponse*) res);
        } else if(res->type() == Response::HtmlResponse) {
            showHtmlResponse((HtmlResponse*) res);
        } else if(res->type() == Response::TextRangesResponse) {
            showTextRangesResponse((TextRangesResponse*) res);
        } else {
            return false;
        }
    } else {
        return false;
    }
    delete res;
    return true;
}

void WebViewForm::showStringResponse(StringResponse *res)
{
    loadStyleSheet();
    m_view->setHtml(res->data());
    actTitle();
}

void WebViewForm::showUrlResponse(UrlResponse *res)
{
    myDebug() << res->url();
    if(!res->blockRules().isEmpty()) {
        //m_view->setBlockRules(res->blockRules());
    }
    m_lastUrl = QUrl(res->url());
    m_view->load(m_lastUrl);
    actTitle();
}

void WebViewForm::showHtmlResponse(HtmlResponse *res)
{
    loadStyleSheet();
    m_view->setHtml(res->data());
    actTitle();
}

void WebViewForm::showTextRangesResponse(TextRangesResponse *res)
{
    Q_UNUSED(res);
    myWarning() << "WebViewForm::showTextRangesResponse not implemented";
}
void WebViewForm::loadStyleSheet(QString url)
{
    if(url.isEmpty()) {
        url = getStyleSheetUrl();
        if(url.isEmpty())
            url = ":/data/css/default.css";
    }
    m_view->insertStyleSheet("style", url, true);
}
QString WebViewForm::getStyleSheetUrl()
{
    myDebug() << "!!! no style sheet url";
    return QString();
}

void WebViewForm::addJS(const QString &url)
{
    QFile file(url);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        return;
    QTextStream stream(&file);
    m_view->page()->runJavaScript(stream.readAll());
    file.close();
}
ModuleID WebViewForm::moduleID() const
{
    return ModuleIDNotSet;
}
