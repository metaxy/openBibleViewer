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
#include "src/core/settings/modulesettings.h"

ModuleSettings::ModuleSettings()
{
    v11n = NULL;
    m_parent = NULL;

    useParentSettings = true;
    zefbible_textFormatting = NewLine;
    zefbible_hardCache = true;
    zefbible_softCache = true;
    biblequote_removeHtml = false;
    styleSheet = "";

}

ModuleSettings::ModuleSettings(ModuleSettings *parent)
{
    v11n = NULL;
    m_parent = parent;
}

ModuleSettings::~ModuleSettings()
{
    //todo: maybe use qsharedpointer
    /*if(v11n != NULL) {
        delete v11n;
        v11n = NULL;
    }*/
    if(m_displaySettings) {
        m_displaySettings.clear();
    }
}

void ModuleSettings::loadVersification()
{
    //DEBUG_FUNC_NAME;
    //myDebug() << "versifiction name = " << versificationName << " versification file" << versificationFile;
    if(versificationName == "kjv") {
        v11n = new Versification_KJV();
        v11n->setFlags(Versification::ReturnAll);
    } else if(versificationName == "kjv-ot") {
        v11n = new Versification_KJV();
        v11n->setFlags(Versification::ReturnOT);
    } else if(versificationName == "kjv-nt") {
        v11n = new Versification_KJV();
        v11n->setFlags(Versification::ReturnNT);
    } else {
        QSettings settings(versificationFile, QSettings::IniFormat);
        const QStringList books = settings.childGroups();
        //if we could not load anything v11n has to be NULL,
        //because eg. BibleQuote will than load the versification from the module files
        if(books.isEmpty())
            return;
        QMap<int, BookV11N> map;
        foreach(const QString & book, books) {
            const int bookID = book.toInt();
            settings.beginGroup(book);
            const QString name = settings.value("name").toString();
            const QStringList shortNames = settings.value("shortNames").toStringList();
            const int maxChapter = settings.value("maxChapter").toInt();
            const QStringList maxVerse = settings.value("maxVerse").toStringList();
            QList<int> iMaxVerse;
            foreach(QString v, maxVerse) {
                iMaxVerse.append(v.toInt());
            }
            BookV11N v;
            v.bookID = bookID;
            v.maxChapter = maxChapter;
            v.maxVerse = iMaxVerse;
            v.name = name;
            v.shortNames = shortNames;
            map.insert(bookID, v);
            settings.endGroup();
        }
        v11n = new Versification_Cache(map);
    }
}

void ModuleSettings::saveVersification()
{
    //DEBUG_FUNC_NAME;
    if(versificationFile != "") {
        //myDebug() << versificationFile;
        QSettings settings(versificationFile, QSettings::IniFormat);
        if(v11n == NULL)
            return;
        const QMap<int, BookV11N> map = v11n->data();
        QMapIterator<int, BookV11N> it(map);
        while(it.hasNext()) {
            it.next();
            settings.beginGroup(QString::number(it.key()));
            BookV11N book = it.value();
            settings.setValue("name", book.name);
            settings.setValue("shortNames", book.shortNames);
            settings.setValue("maxChapter", book.maxChapter);
            QStringList maxVerse;
            foreach(int v, book.maxVerse) {
                maxVerse << QString::number(v);
            }
            settings.setValue("maxVerse", maxVerse);
            settings.endGroup();
        }
    }
}

Versification *ModuleSettings::getV11n()
{
    if(!v11n) {
        loadVersification();
    }
    return v11n;

}

void ModuleSettings::setParent(ModuleSettings *parent)
{
    m_parent = parent;
}

ModuleSettings *ModuleSettings::parent() const
{
    return m_parent;
}

QList<ModuleSettings *> ModuleSettings::children() const
{
    return m_children;
}

void ModuleSettings::appendChild(ModuleSettings* child)
{
    m_children.append(child);
}

void ModuleSettings::removeChild(ModuleSettings *child)
{
    m_children.removeOne(child);
}

void ModuleSettings::clearChildren()
{
    m_children.clear();
}

QString ModuleSettings::name(bool preferShortName) const
{
    if(preferShortName) {
        if(!moduleShortName.isEmpty()) {
            return moduleShortName;
        } else {
            return moduleName;
        }
    } else {
        if(!moduleName.isEmpty()) {
            return moduleName;
        } else {
            return moduleShortName;
        }
    }
}

QSharedPointer<ModuleDisplaySettings> ModuleSettings::displaySettings() const
{
    return m_displaySettings;
}

void ModuleSettings::setDisplaySettings(QSharedPointer<ModuleDisplaySettings> settings)
{
    m_displaySettings = settings;
}

void ModuleSettings::removeDisplaySettings()
{
    if(m_displaySettings != NULL) {
        m_displaySettings.clear();
    }

}
