/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
    m_parent = parent;
}

ModuleSettings::~ModuleSettings()
{
    v11n.clear();

    if(m_displaySettings) {
        m_displaySettings.clear();
    }
}

QSharedPointer<Versification> ModuleSettings::loadVersification()
{
    DEBUG_FUNC_NAME;
    QSharedPointer<Versification> ret;
    //myDebug() << "versifiction name = " << versificationName << " versification file" << versificationFile;
    if(versificationName == "kjv") {
        ret = QSharedPointer<Versification>(new Versification_KJV());
        ret->setFlags(Versification::ReturnAll);
    } else if(versificationName == "kjv-ot") {
        ret = QSharedPointer<Versification>(new Versification_KJV());
        ret->setFlags(Versification::ReturnOT);
    } else if(versificationName == "kjv-nt") {
        ret = QSharedPointer<Versification>(new Versification_KJV());
        ret->setFlags(Versification::ReturnNT);
    } else {
        QSettings settings(versificationFile, QSettings::IniFormat);
        const QStringList books = settings.childGroups();
        //if we could not load anything v11n has to be NULL,
        //because eg. BibleQuote or ZefaniaBible will than load the versification from the module files
        if(books.isEmpty())
            return ret;
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
        ret = QSharedPointer<Versification>(new Versification_Cache(map));
    }
    v11n = ret.toWeakRef();
    return ret;
}

void ModuleSettings::saveVersification()
{
    //DEBUG_FUNC_NAME;
    if(versificationFile != "") {
        QSettings settings(versificationFile, QSettings::IniFormat);
        if(v11n.isNull())
            return;
        const QMap<int, BookV11N> map = v11n.data()->data();
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

QSharedPointer<Versification> ModuleSettings::getV11n()
{
    DEBUG_FUNC_NAME
    if(v11n.isNull()) {
        myDebug() << "loading new";
        return loadVersification();
    }
    myDebug() << v11n;
    return v11n.toStrongRef();

}
bool ModuleSettings::noV11n()
{
    DEBUG_FUNC_NAME
    return v11n.isNull();
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
    //myDebug() << moduleShortName << moduleName;
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
