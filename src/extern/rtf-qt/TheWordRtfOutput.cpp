/*
    Copyright (C)  2010  Brad Hards <bradh@frogmouth.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TheWordRtfOutput.h"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QUuid>
#include <QtGui/QTextDocumentFragment>
#include "src/core/link/verseurl.h"
#include "src/core/verse/reftext.h"
namespace RtfReader
{
TheWordRtfOutput::TheWordRtfOutput(QTextDocument *document, const QString &title) : TextDocumentRtfOutput(document),
    m_document(document), m_haveSetFont(false)
{
    m_cursor = new QTextCursor(m_document);

    if(!title.isEmpty()) {
        m_cursor->insertBlock();
        QTextCharFormat t;
        t.setFontWeight(QFont::Bold);
        m_cursor->insertText(title, t);
        m_cursor->insertBlock();

    }

    QTextCharFormat defaultCharFormat;
    defaultCharFormat.setFontPointSize(12);   // default of 24 "half-points"
    m_textCharFormatStack.push(defaultCharFormat);
}

TheWordRtfOutput::~TheWordRtfOutput()
{
    delete m_cursor;
}

void TheWordRtfOutput::appendText(const QString &text)
{
    m_cursor->insertText(text, m_textCharFormatStack.top());
}

void TheWordRtfOutput::appendLink(const QString &href, const QString &text)
{
    QString t = text;
    QString href2 = href;
    href2.remove(0,1).chop(1);

    if(t == "[vref]") {
        VerseUrl url;
        url.fromTheWord(href2);
        RefText ref(m_settings);
        t = ref.toString(url);
    }

    QTextCharFormat format;
    format.setAnchorHref(href2);
    format.setAnchorName(t);
    format.setAnchor(true);
    m_cursor->insertText(t, format);


}

void TheWordRtfOutput::setSettings(Settings *settings)
{
    m_settings = settings;
}
}
