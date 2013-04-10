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

#ifndef RTFREADER_ESwordRtfOutput_H
#define RTFREADER_ESwordRtfOutput_H

#include "AbstractRtfOutput.h"

class QImage;
class QTextCursor;
class QTextDocument;
class QTextImageFormat;

#include <QStack>
#include <QTextCharFormat>
#include "TheWordRtfOutput.h"

namespace RtfReader
{
class Reader;

class ESwordRtfOutput : public TextDocumentRtfOutput
{
public:
    ESwordRtfOutput(QTextDocument *document);

    virtual ~ESwordRtfOutput();
    virtual void setFontUnderline(const int value);
    virtual void appendLink(const QString &href, const QString &text);
    virtual void appendText(const QString &text);
    void setSettings(Settings *settings);

protected:
    QTextDocument *m_document;
    bool m_haveSetFont;
    Settings *m_settings;
    void appendLink2(const QString &href, const QString &text);

};
}

#endif
