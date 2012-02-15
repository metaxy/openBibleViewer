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

#include "HtmlOutput.h"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QUuid>

namespace RtfReader
{
HtmlOutput::HtmlOutput(QTextDocument *document) : AbstractRtfOutput(),
    m_document(document), m_haveSetFont(false)
{
    m_cursor = new QTextCursor(m_document);
    QTextCharFormat defaultCharFormat;
    defaultCharFormat.setFontPointSize(12);   // default of 24 "half-points"
    m_textCharFormatStack.push(defaultCharFormat);
}

HtmlOutput::~HtmlOutput()
{
    delete m_cursor;
}

void HtmlOutput::startGroup()
{
    if(! m_haveSetFont) {
        // TODO: think harder about how to deal with default font cases.
        setFont(m_defaultFontIndex);
    }
    QTextCharFormat charFormat = m_textCharFormatStack.top(); // inherit all current properties
    m_textCharFormatStack.push(charFormat);
}

void HtmlOutput::endGroup()
{
    m_textCharFormatStack.pop();
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::appendText(const QString &text)
{
    m_cursor->insertText(text);
}

void HtmlOutput::insertPar()
{
    m_cursor->insertBlock();
}

void HtmlOutput::insertTab()
{
    m_cursor->insertText("\t");
}

void HtmlOutput::insertLeftQuote()
{
    m_cursor->insertText(QChar(0x2018));
}

void HtmlOutput::insertRightQuote()
{
    m_cursor->insertText(QChar(0x2019));
}

void HtmlOutput::insertLeftDoubleQuote()
{
    m_cursor->insertText(QChar(0x201c));
}

void HtmlOutput::insertRightDoubleQuote()
{
    m_cursor->insertText(QChar(0x201d));
}

void HtmlOutput::insertEnDash()
{
    m_cursor->insertText(QChar(0x2013));
}

void HtmlOutput::insertEmDash()
{
    m_cursor->insertText(QChar(0x2014));
}

void HtmlOutput::insertEmSpace()
{
    m_cursor->insertText(QChar(0x2003));
}

void HtmlOutput::insertEnSpace()
{
    m_cursor->insertText(QChar(0x2002));
}

void HtmlOutput::insertBullet()
{
    m_cursor->insertText(QChar(0x2022));
}

void HtmlOutput::setFontItalic(const int value)
{
    m_textCharFormatStack.top().setFontItalic(value != 0);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setFontBold(const int value)
{
    int weight = QFont::Normal;
    if(value != 0) {
        weight = QFont::Bold;
    }
    m_textCharFormatStack.top().setFontWeight(weight);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setFontUnderline(const int value)
{
    m_textCharFormatStack.top().setFontUnderline(value != 0);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setFontPointSize(const int pointSize)
{
    m_textCharFormatStack.top().setFontPointSize(pointSize);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setForegroundColour(const int colourIndex)
{
    QColor colour = m_colourTable.value(colourIndex);
    if(colour.isValid()) {
        m_textCharFormatStack.top().setForeground(colour);
        m_cursor->setCharFormat(m_textCharFormatStack.top());
    } else {
        qDebug() << "invalid colour at index:" << colourIndex;
    }
}

void HtmlOutput::setHighlightColour(const int colourIndex)
{
    QColor colour = m_colourTable.value(colourIndex);
    if(colour.isValid()) {
        m_textCharFormatStack.top().setBackground(colour);
        m_cursor->setCharFormat(m_textCharFormatStack.top());
    } else {
        qDebug() << "invalid colour at index:" << colourIndex;
    }
}

void HtmlOutput::setParagraphPatternBackgroundColour(const int colourIndex)
{
    QColor colour = m_colourTable.value(colourIndex);
    if(colour.isValid()) {
        m_paragraphFormat.setBackground(colour);
        m_cursor->setBlockFormat(m_paragraphFormat);
    } else {
        qDebug() << "invalid colour at index:" << colourIndex;
    }
}

void HtmlOutput::setFont(const int fontIndex)
{
    if(! m_fontTable.contains(fontIndex)) {
        //qDebug() << "attempted to select fontIndex" << fontIndex << "not in the font table";
        return;
    }
    FontTableEntry fontEntry = m_fontTable.value(fontIndex);
    //qDebug() << "selecting font:" << fontEntry.fontName();
    m_textCharFormatStack.top().setFontFamily(fontEntry.fontName());
    m_cursor->setCharFormat(m_textCharFormatStack.top());
    m_haveSetFont = true;
}

void HtmlOutput::setDefaultFont(const int fontIndex)
{
    m_defaultFontIndex = fontIndex;
}

void HtmlOutput::appendToColourTable(const QColor &colour)
{
    m_colourTable.append(colour);
}

void HtmlOutput::insertFontTableEntry(FontTableEntry fontTableEntry, quint32 fontTableIndex)
{
    // qDebug() << "inserting font entry:" << fontTableIndex << "with name:" << fontTableEntry.fontName();
    m_fontTable.insert(fontTableIndex, fontTableEntry);
}

void HtmlOutput::insertStyleSheetTableEntry(quint32 stylesheetTableIndex, StyleSheetTableEntry stylesheetTableEntry)
{
    //qDebug() << "inserting stylesheet entry:" << stylesheetTableIndex << "with name:" << stylesheetTableEntry.styleName();
    m_stylesheetTable.insert(stylesheetTableIndex, stylesheetTableEntry);
}

void HtmlOutput::resetParagraphFormat()
{
    m_paragraphFormat.setAlignment(Qt::AlignLeft);
    m_paragraphFormat.setTextIndent(0);
    m_paragraphFormat.setLeftMargin(0);
    m_paragraphFormat.setRightMargin(0);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::resetCharacterProperties()
{
    m_textCharFormatStack.top().setFontPointSize(12);   // default of 24 "half-points"
    m_textCharFormatStack.top().setFontWeight(QFont::Normal);
    m_textCharFormatStack.top().setFontItalic(false);
    m_textCharFormatStack.top().setFontUnderline(false);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setParagraphAlignmentLeft()
{
    m_paragraphFormat.setAlignment(Qt::AlignLeft);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setParagraphAlignmentCentred()
{
    m_paragraphFormat.setAlignment(Qt::AlignHCenter);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setParagraphAlignmentJustified()
{
    m_paragraphFormat.setAlignment(Qt::AlignJustify);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setParagraphAlignmentRight()
{
    m_paragraphFormat.setAlignment(Qt::AlignRight);
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setLeftIndent(const int twips)
{
    m_paragraphFormat.setLeftMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setRightIndent(const int twips)
{
    m_paragraphFormat.setRightMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setSpaceBefore(const int twips)
{
    m_paragraphFormat.setTopMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setSpaceAfter(const int twips)
{
    m_paragraphFormat.setBottomMargin(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setFirstLineIndent(const int twips)
{
    m_paragraphFormat.setTextIndent(pixelsFromTwips(twips));
    m_cursor->setBlockFormat(m_paragraphFormat);
}

void HtmlOutput::setFontSuperscript()
{
    m_textCharFormatStack.top().setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setFontSubscript()
{
    m_textCharFormatStack.top().setVerticalAlignment(QTextCharFormat::AlignSubScript);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setTextDirectionLeftToRight()
{
    m_textCharFormatStack.top().setLayoutDirection(Qt::LeftToRight);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::setTextDirectionRightToLeft()
{
    m_textCharFormatStack.top().setLayoutDirection(Qt::RightToLeft);
    m_cursor->setCharFormat(m_textCharFormatStack.top());
}

void HtmlOutput::createImage(const QImage &image, const QTextImageFormat &format)
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

void HtmlOutput::setPageHeight(const int pageHeight)
{
    qDebug() << "setPageHeight: " << pageHeight << " (" << pageHeight / 1440.0 << ")";
}

void HtmlOutput::setPageWidth(const int pageWidth)
{
    qDebug() << "setPageWidth: " << pageWidth << " (" << pageWidth / 1440.0 << ")";
}

qreal HtmlOutput::pixelsFromTwips(const int twips)
{
    qreal inches = twips / 1440.0;
    qreal pixels = inches * 96.0;
    return pixels;
}
void HtmlOutput::appendLink(const QString &href, const QString &text)
{
    m_cursor->insertHtml("<a href="+href+">"+text +"</a>");
}
}
