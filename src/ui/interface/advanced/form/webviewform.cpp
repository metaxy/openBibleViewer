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
#include <QtCore/QPointer>
#include <QPrinter>
#include <QFileInfo>
#include <QTextStream>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QFileDialog>
#include <QTextDocumentWriter>
WebViewForm::WebViewForm(QWidget *parent) :
    Form(parent)
{
    m_view = new WebView(this);
    m_view->setObjectName("webView");
    m_view->setUrl(QUrl("about:blank"));
}
void WebViewForm::copy()
{
    m_view->page()->triggerAction(QWebPage::Copy);
}

void WebViewForm::selectAll()
{
    m_view->page()->triggerAction(QWebPage::SelectAll);
}

void WebViewForm::print()
{
    QPrinter printer;
    QPointer<QPrintDialog> dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print"));
    if(dialog->exec() == QDialog::Accepted) {
        m_view->page()->mainFrame()->print(&printer);
    }
    delete dialog;
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
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << m_view->page()->mainFrame()->toHtml();
        file.close();
    } else if(fi.suffix().compare("pdf", Qt::CaseInsensitive) == 0) {
        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        m_view->print(&printer);
    } else if(fi.suffix().compare("odt", Qt::CaseInsensitive) == 0) {
        QTextDocumentWriter writer;
        writer.setFormat("odf");
        writer.setFileName(fileName);
        QTextDocument doc;
        doc.setHtml(m_view->page()->mainFrame()->toHtml());
        writer.write(&doc);
    } else {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        out << m_view->page()->mainFrame()->toPlainText();
        file.close();
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
