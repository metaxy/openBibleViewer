/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include <QtGui/QtGui>

#include "xbelreader.h"

XbelReader::XbelReader(QTreeWidget *treeWidget)
        : treeWidget(treeWidget)
{
    QStyle *style = treeWidget->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileLinkIcon));
}

bool XbelReader::read(QIODevice *device)
{
    setDevice(device);
    while (!atEnd()) {
        readNext();
        if (isStartElement()) {
            if (name() == "xbel" && attributes().value("version") == "1.0")
                readXBEL();
            else
                raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
        }
    }
    return !error();
}

void XbelReader::readUnknownElement()
{

    Q_ASSERT(isStartElement());
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement())
            readUnknownElement();
    }

}

void XbelReader::readXBEL()
{

    Q_ASSERT(isStartElement() && name() == "xbel");
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "folder")
                readFolder(0);
            else if (name() == "bookmark")
                readBookmark(0);
            else if (name() == "separator")
                readSeparator(0);
            else
                readUnknownElement();
        }
    }

}

void XbelReader::readTitle(QTreeWidgetItem *item)
{
    Q_ASSERT(isStartElement() && name() == "title");

    QString title = readElementText();
    item->setText(0, title);
}

void XbelReader::readSeparator(QTreeWidgetItem *item)
{

    QTreeWidgetItem *separator = createChildItem(item);
    separator->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    separator->setText(0, QString(30, 0xB7));
    readElementText();

}

void XbelReader::readFolder(QTreeWidgetItem *item)
{

    Q_ASSERT(isStartElement() && name() == "folder");

    QTreeWidgetItem *folder = createChildItem(item);

    folder->setFlags(folder->flags() | Qt::ItemIsEditable);
    QStyle *style = treeWidget->style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
    folder->setIcon(0, folderIcon);

    bool folded = (attributes().value("folded") != "no");
    treeWidget->setItemExpanded(folder, !folded);

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "title")
                readTitle(folder);
            else if (name() == "folder")
                readFolder(folder);
            else if (name() == "bookmark")
                readBookmark(folder);
            else if (name() == "separator")
                readSeparator(folder);
            else
                readUnknownElement();
        }
    }

}

void XbelReader::readBookmark(QTreeWidgetItem *item)
{

    Q_ASSERT(isStartElement() && name() == "bookmark");

    QTreeWidgetItem *bookmark = createChildItem(item);
    bookmark->setFlags(bookmark->flags() | Qt::ItemIsEditable);
    bookmark->setIcon(0, bookmarkIcon);
    bookmark->setText(0, QObject::tr("Unknown title"));
    bookmark->setText(1, attributes().value("href").toString());
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "title")
                readTitle(bookmark);
            else
                readUnknownElement();
        }
    }

}

QTreeWidgetItem *XbelReader::createChildItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, name().toString());
    return childItem;
}
