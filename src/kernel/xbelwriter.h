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

 #ifndef XBELWRITER_H
 #define XBELWRITER_H

 #include <QXmlStreamWriter>

 class QTreeWidget;
 class QTreeWidgetItem;

 class XbelWriter : public QXmlStreamWriter
 {
 public:
	 XbelWriter(QTreeWidget *treeWidget);
	 bool writeFile(QIODevice *device);

 private:
	 QTreeWidget *treeWidget;
	 void writeItem(QTreeWidgetItem *item);
 };

 #endif
