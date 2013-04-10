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

#ifndef RTFREADER_TheWordRtfOutput_H
#define RTFREADER_TheWordRtfOutput_H

#include "AbstractRtfOutput.h"

class QImage;
class QTextCursor;
class QTextDocument;
class QTextImageFormat;

#include <QStack>
#include <QTextCharFormat>
#include "TextDocumentRtfOutput.h"
#include "src/core/settings/settings.h"

namespace RtfReader
{
class Reader;

class TheWordRtfOutput : public TextDocumentRtfOutput
{
public:
    TheWordRtfOutput(QTextDocument *document, const QString &title);

    virtual ~TheWordRtfOutput();
    virtual void appendText(const QString &text);
    virtual void appendLink(const QString &href, const QString &text);

    void setSettings(Settings *settings);

protected:
    QTextDocument *m_document;

    Settings *m_settings;

    bool m_haveSetFont;
};
}

#endif
