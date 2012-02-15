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
TheWordRtfOutput::TheWordRtfOutput(QTextDocument *document) : AbstractRtfOutput(),
    m_document(document), m_haveSetFont(false)
{
    m_cursor = new QTextCursor(m_document);
    m_cursor->movePosition(QTextCursor::End);
    QTextCharFormat defaultCharFormat;
    defaultCharFormat.setFontPointSize(12);   // default of 24 "half-points"
    m_textCharFormatStack.push(defaultCharFormat);
}

TheWordRtfOutput::~TheWordRtfOutput()
{
    delete m_cursor;
}

void TheWordRtfOutput::startGroup()
{
    if(! m_haveSetFont) {
        // TODO: think harder about how to deal with default font cases.
        setFont(m_defaultFontIndex);
    }
    QTextCharFormat charFormat = m_textCharFormatStack.top(); // inherit all current properties
    m_textCharFormatStack.push(charFormat);
}

void TheWordRtfOutput::endGroup()
{
    m_textCharFormatStack.pop();
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::appendText(const QString &text)
{
    m_cursor->insertText(text);
}

void TheWordRtfOutput::insertPar()
{
    m_cursor->insertBlock();
}

void TheWordRtfOutput::insertTab()
{
    m_cursor->insertText("\t");
}

void TheWordRtfOutput::insertLeftQuote()
{
    m_cursor->insertText(QChar(0x2018));
}

void TheWordRtfOutput::insertRightQuote()
{
    m_cursor->insertText(QChar(0x2019));
}

void TheWordRtfOutput::insertLeftDoubleQuote()
{
    m_cursor->insertText(QChar(0x201c));
}

void TheWordRtfOutput::insertRightDoubleQuote()
{
    m_cursor->insertText(QChar(0x201d));
}

void TheWordRtfOutput::insertEnDash()
{
    m_cursor->insertText(QChar(0x2013));
}

void TheWordRtfOutput::insertEmDash()
{
    m_cursor->insertText(QChar(0x2014));
}

void TheWordRtfOutput::insertEmSpace()
{
    m_cursor->insertText(QChar(0x2003));
}

void TheWordRtfOutput::insertEnSpace()
{
    m_cursor->insertText(QChar(0x2002));
}

void TheWordRtfOutput::insertBullet()
{
    m_cursor->insertText(QChar(0x2022));
}

void TheWordRtfOutput::setFontItalic(const int value)
{
    m_textCharFormatStack.top().setFontItalic(value != 0);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setFontBold(const int value)
{
    int weight = QFont::Normal;
    if(value != 0) {
        weight = QFont::Bold;
    }
    m_textCharFormatStack.top().setFontWeight(weight);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setFontUnderline(const int value)
{
    m_textCharFormatStack.top().setFontUnderline(value != 0);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setFontPointSize(const int pointSize)
{
    m_textCharFormatStack.top().setFontPointSize(pointSize);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setForegroundColour(const int colourIndex)
{
    QColor colour = m_colourTable.value(colourIndex);
    if(colour.isValid()) {
        m_textCharFormatStack.top().setForeground(colour);
        m_cursor->setCharFormat(m_textCharFormatStack.top());
    } else {
        qDebug() << "invalid colour at index:" << colourIndex;
    }
}

void TheWordRtfOutput::setHighlightColour(const int colourIndex)
{
    QColor colour = m_colourTable.value(colourIndex);
    if(colour.isValid()) {
        m_textCharFormatStack.top().setBackground(colour);
        m_cursor->setCharFormat(m_textCharFormatStack.top());
    } else {
        qDebug() << "invalid colour at index:" << colourIndex;
    }
}

void TheWordRtfOutput::setParagraphPatternBackgroundColour(const int colourIndex)
{
    QColor colour = m_colourTable.value(colourIndex);
    if(colour.isValid()) {
        m_paragraphFormat.setBackground(colour);
        m_cursor->setBlockFormat(m_paragraphFormat);
    } else {
        qDebug() << "invalid colour at index:" << colourIndex;
    }
}

void TheWordRtfOutput::setFont(const int fontIndex)
{
    if(! m_fontTable.contains(fontIndex)) {
        qDebug() << "attempted to select fontIndex" << fontIndex << "not in the font table";
        return;
    }
    FontTableEntry fontEntry = m_fontTable.value(fontIndex);
    qDebug() << "selecting font:" << fontEntry.fontName();
    m_textCharFormatStack.top().setFontFamily(fontEntry.fontName());
    m_cursor->setCharFormat(m_textCharFormatStack.top());
    m_haveSetFont = true;
}

void TheWordRtfOutput::setDefaultFont(const int fontIndex)
{
    m_defaultFontIndex = fontIndex;
}

void TheWordRtfOutput::appendToColourTable(const QColor &colour)
{
    m_colourTable.append(colour);
}

void TheWordRtfOutput::insertFontTableEntry(FontTableEntry fontTableEntry, quint32 fontTableIndex)
{
    // qDebug() << "inserting font entry:" << fontTableIndex << "with name:" << fontTableEntry.fontName();
    m_fontTable.insert(fontTableIndex, fontTableEntry);
}

void TheWordRtfOutput::insertStyleSheetTableEntry(quint32 stylesheetTableIndex, StyleSheetTableEntry stylesheetTableEntry)
{
    qDebug() << "inserting stylesheet entry:" << stylesheetTableIndex << "with name:" << stylesheetTableEntry.styleName();
    m_stylesheetTable.insert(stylesheetTableIndex, stylesheetTableEntry);
}

void TheWordRtfOutput::resetParagraphFormat()
{
    m_paragraphFormat.setAlignment(Qt::AlignLeft);
    m_paragraphFormat.setTextIndent(0);
    m_paragraphFormat.setLeftMargin(0);
    m_paragraphFormat.setRightMargin(0);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::resetCharacterProperties()
{
    m_textCharFormatStack.top().setFontPointSize(12);   // default of 24 "half-points"
    m_textCharFormatStack.top().setFontWeight(QFont::Normal);
    m_textCharFormatStack.top().setFontItalic(false);
    m_textCharFormatStack.top().setFontUnderline(false);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setParagraphAlignmentLeft()
{
    m_paragraphFormat.setAlignment(Qt::AlignLeft);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setParagraphAlignmentCentred()
{
    m_paragraphFormat.setAlignment(Qt::AlignHCenter);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setParagraphAlignmentJustified()
{
    m_paragraphFormat.setAlignment(Qt::AlignJustify);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setParagraphAlignmentRight()
{
    m_paragraphFormat.setAlignment(Qt::AlignRight);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setLeftIndent(const int twips)
{
    m_paragraphFormat.setLeftMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setRightIndent(const int twips)
{
    m_paragraphFormat.setRightMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setSpaceBefore(const int twips)
{
    m_paragraphFormat.setTopMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setSpaceAfter(const int twips)
{
    m_paragraphFormat.setBottomMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setFirstLineIndent(const int twips)
{
    m_paragraphFormat.setTextIndent(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void TheWordRtfOutput::setFontSuperscript()
{
    m_textCharFormatStack.top().setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setFontSubscript()
{
    m_textCharFormatStack.top().setVerticalAlignment(QTextCharFormat::AlignSubScript);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setTextDirectionLeftToRight()
{
    m_textCharFormatStack.top().setLayoutDirection(Qt::LeftToRight);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::setTextDirectionRightToLeft()
{
    m_textCharFormatStack.top().setLayoutDirection(Qt::RightToLeft);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void TheWordRtfOutput::createImage(const QImage &image, const QTextImageFormat &format)
{
#if 0
    QString imageUuid = QString("rtfparser://") + QUuid::createUuid().toString();
    m_document->addResource(QTextDocument::ImageResource, QUrl(imageUuid), QVariant(image));
    format.setName(imageUuid);
    m_cursor->insertImage(format);
#else
    m_cursor->insertImage(image.scaled(format.width(), format.height()));
#endif
}

void TheWordRtfOutput::setPageHeight(const int pageHeight)
{
    qDebug() << "setPageHeight: " << pageHeight << " (" << pageHeight / 1440.0 << ")";
}

void TheWordRtfOutput::setPageWidth(const int pageWidth)
{
    qDebug() << "setPageWidth: " << pageWidth << " (" << pageWidth / 1440.0 << ")";
}

qreal TheWordRtfOutput::pixelsFromTwips(const int twips)
{
    qreal inches = twips / 1440.0;
    qreal pixels = inches * 96.0;
    return pixels;
}
void TheWordRtfOutput::appendLink(const QString &href, const QString &text)
{
    QString t = text;
    if(text == "[vref]") {
        QString href2 = href;
        href2.remove(0,1).chop(1);
        VerseUrl url;
        url.fromTheWord(href2);
        url.setModuleID(m_settings->getDefaultModule(ModuleTools::BibleContent));
        RefText ref;
        ref.setSettings(m_settings);
        t = ref.toString(url);
    }
    m_cursor->insertHtml("<a href="+href+">"+t+"</a>");
}

void TheWordRtfOutput::setSettings(Settings *settings)
{
    m_settings = settings;
}
}
