/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include "moduleapi.h"
#include "src/core/dbghelper.h"
#include <QWebElementCollection>
ModuleApi::ModuleApi(QObject *parent) :
    QObject(parent)
{
}
ModuleApi::~ModuleApi()
{

}

void ModuleApi::activateModule(const int verseTableID)
{
    //DEBUG_FUNC_NAME;
    //myDebug() << verseTableID;

    m_actions->setCurrentVerseTableID(verseTableID);

    QWebElementCollection collection = m_frame->documentElement().findAll("td[class~=verseTableTitle]");
    foreach(QWebElement paraElement, collection) {
        paraElement.removeClass("active");
        if(paraElement.attribute("verseTableID") == QString::number(verseTableID)) {
            paraElement.addClass("active");
        }
    }
    m_actions->reloadActive();
}
void ModuleApi::setFrame(QWebFrame *frame)
{
    m_frame = frame;
}
