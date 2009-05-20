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
#ifndef XBELREADER_H
#define XBELREADER_H

#include <QIcon>
#include <QXmlStreamReader>

class QTreeWidget;
class QTreeWidgetItem;

class XbelReader : public QXmlStreamReader
{
public:
        XbelReader(QTreeWidget *treeWidget);

        bool read(QIODevice *device);

private:
        void readUnknownElement();
        void readXBEL();
        void readTitle(QTreeWidgetItem *item);
        void readSeparator(QTreeWidgetItem *item);
        void readFolder(QTreeWidgetItem *item);
        void readBookmark(QTreeWidgetItem *item);

        QTreeWidgetItem *createChildItem(QTreeWidgetItem *item);

        QTreeWidget *treeWidget;

        QIcon folderIcon;
        QIcon bookmarkIcon;
};

#endif
