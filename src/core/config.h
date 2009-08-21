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
#ifndef CONFIG_H
#define CONFIG_H
#include "moduleconfig.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
struct settings_s
{
	QString encoding;
	QList<moduleConfig> module;
	QMap<int,int> moduleID;
	qreal zoomstep;
	QString version;
	QString build;
	bool removeHtml;
	QString language;
	int autoLayout;// 0 = NONE, 1 = vertical tile ,2 = horizontal tile, 3 = cascade
	bool onClickBookmarkGo;
	int textFormatting;//0 = Neue Zeile nach Vers, 1 = Unformatierter Textblock
	bool zefaniaBible_hardCache;
	bool zefaniaBible_softCache;
	QString homePath;
};
#endif // CONFIG_H
