/***************************************************************************
openBibleViewer - Free Bibel Viewer
Copyright (C) 2009 Paul Walger
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

#include <QtGui>
#include "xbelwriter.h"

XbelWriter::XbelWriter(QTreeWidget *treeWidget) : treeWidget(treeWidget)
{
        setAutoFormatting(true);
}

bool XbelWriter::writeFile(QIODevice *device)
{
        setDevice(device);

        writeStartDocument();
        writeDTD("<!DOCTYPE xbel>");
        writeStartElement("xbel");
        writeAttribute("version", "1.0");
        for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
                writeItem(treeWidget->topLevelItem(i));

        writeEndDocument();
        return true;
}

void XbelWriter::writeItem(QTreeWidgetItem *item)
{
        QString tagName = item->data(0, Qt::UserRole).toString();
        if (tagName == "folder")
        {
                bool folded = !treeWidget->isItemExpanded(item);
                writeStartElement(tagName);
                writeAttribute("folded", folded ? "yes" : "no");
                writeTextElement("title", item->text(0));
                for (int i = 0; i < item->childCount(); ++i)
                        writeItem(item->child(i));
                writeEndElement();
        }
        else if (tagName == "bookmark")
        {
                writeStartElement(tagName);
                if (!item->text(1).isEmpty())
                        writeAttribute("href", item->text(1));

                writeTextElement("title", item->text(0));
                writeEndElement();
        }
        else if (tagName == "separator")
        {
                writeEmptyElement(tagName);
        }
}
