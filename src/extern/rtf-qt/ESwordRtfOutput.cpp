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

#include "ESwordRtfOutput.h"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QUuid>
#include "src/core/link/verseurl.h"
namespace RtfReader
{
ESwordRtfOutput::ESwordRtfOutput(QTextDocument *document) : TextDocumentRtfOutput(document),
    m_document(document), m_haveSetFont(false)
{
}

ESwordRtfOutput::~ESwordRtfOutput()
{
    delete m_cursor;
}


void ESwordRtfOutput::setFontUnderline(const int value)
{
    /*m_textCharFormatStack.top().setFontUnderline(value != 0);
    m_cursor->setCharFormat(m_textCharFormatStack.top());*/
}

void ESwordRtfOutput::appendLink(const QString &href, const QString &text)
{
    QString t = text;
    QString href2 = href;
    href2.remove(0,1).chop(1);
    QTextCharFormat format;
    format.setAnchorHref(href2);
    format.setAnchorName(t);
    format.setAnchor(true);
    m_cursor->insertText(t, format);
}

}
