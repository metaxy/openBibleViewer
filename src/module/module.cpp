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
#include "src/module/module.h"
#include "src/module/bible/biblequote.h"
#include "src/module/bible/zefania-bible.h"
#include "src/module/bible/thewordbible.h"
#include "src/module/bible/swordbible.h"
#include "src/module/dictionary/biblequote-dict.h"
#include "src/module/dictionary/zefania-lex.h"
#include "src/module/dictionary/webdictionary.h"
Module::Module(Module *parent) : m_parent(parent)
{
    m_moduleClass = OBVCore::NoneClass;
    m_moduleType = OBVCore::NoneType;

    if(m_parent != NULL) {
        m_parent->append(this);
    }
}
Module::~Module()
{
    m_bibleModule.clear();
    m_dictionaryModule.clear();
}

void Module::setSettings(Settings *settings)
{
    m_settings = settings;
}

Module* Module::parent() const
{
    return m_parent;
}

void Module::append(Module* child)
{
    m_children.append(child);
}

QString Module::path() const
{
    return m_path;
}
int Module::moduleID() const
{
    return m_id;
}
QString Module::moduleUID() const
{
    return m_settings->savableUrl(this->path());
}
OBVCore::ModuleClass Module::moduleClass() const
{
    return m_moduleClass;
}
OBVCore::ModuleType Module::moduleType() const
{
    return m_moduleType;
}

void Module::setPath(const QString &path)
{
    m_path = path;
}

void Module::setModuleID(const int moduleID)
{
    //myDebug() << "new id = " << moduleID;
    m_id = moduleID;
}

void Module::setModuleClass(const OBVCore::ModuleClass &c)
{
    m_moduleClass = c;
}

void Module::setModuleType(const OBVCore::ModuleType &t)
{
    m_moduleType = t;
}
QStringList Module::moduleTypeNames()
{

    QStringList l;
    l << QT_TRANSLATE_NOOP("Core", "None") << QT_TRANSLATE_NOOP("Core", "BibleQuote") << QT_TRANSLATE_NOOP("Core", "Zefania XML Bible")
      << QT_TRANSLATE_NOOP("Core", "Zefania Lex Module") << QT_TRANSLATE_NOOP("Core", "BibleQuote Dictionary") << QT_TRANSLATE_NOOP("Core", "The Word Bible")
      << QT_TRANSLATE_NOOP("Core", "Folder")
      << QT_TRANSLATE_NOOP("Core", "Sword Bible") << QT_TRANSLATE_NOOP("Core", "Web Page") << QT_TRANSLATE_NOOP("Core", "Web Dictionary");
    return l;
}

QString Module::moduleTypeName(OBVCore::ModuleType type)
{
    if(type == OBVCore::NoneType) {
        return QT_TRANSLATE_NOOP("Core", "None");
    } else if(type == OBVCore::BibleQuoteModule) {
        return QT_TRANSLATE_NOOP("Core", "BibleQuote");
    } else if(type == OBVCore::ZefaniaBibleModule) {
        return QT_TRANSLATE_NOOP("Core", "Zefania XML Bible");
    } else if(type == OBVCore::ZefaniaLexModule) {
        return QT_TRANSLATE_NOOP("Core", "Zefania Lex Module");
    } else if(type == OBVCore::BibleQuoteDictModule) {
        return QT_TRANSLATE_NOOP("Core", "BibleQuote Dictionary");
    } else if(type == OBVCore::TheWordBibleModule) {
        return QT_TRANSLATE_NOOP("Core", "The Word Bible");
    } else if(type == OBVCore::SwordBibleModule) {
        return QT_TRANSLATE_NOOP("Core", "Sword Bible");
    } else if(type == OBVCore::WebPageModule) {
        return QT_TRANSLATE_NOOP("Core", "Web Page");
    } else if(type == OBVCore::WebDictionaryModule) {
        return QT_TRANSLATE_NOOP("Core", "Web Dictionary");
    } else if(type == OBVCore::FolderModule) {
        return QT_TRANSLATE_NOOP("Core", "Folder");
    }
    return "";
}

QSharedPointer<BibleModule> Module::newBibleModule(const OBVCore::ModuleType type)
{
    QSharedPointer<BibleModule> ret;
    if(type == OBVCore::ZefaniaBibleModule) {
        ret = QSharedPointer<BibleModule>(new ZefaniaBible());
    } else if(type == OBVCore::TheWordBibleModule) {
        ret = QSharedPointer<BibleModule>(new TheWordBible());
    } else if(type == OBVCore::SwordBibleModule) {
        ret = QSharedPointer<BibleModule>(new SwordBible());
    } else if(type == OBVCore::BibleQuoteModule) {
        ret = QSharedPointer<BibleModule>(new BibleQuote());
    }
    m_bibleModule = ret.toWeakRef();
    return ret;
}
QSharedPointer<DictionaryModule> Module::newDictionaryModule(const OBVCore::ModuleType type)
{
    QSharedPointer<DictionaryModule> ret;
    if(type == OBVCore::ZefaniaLexModule) {
        ret = QSharedPointer<DictionaryModule>(new ZefaniaLex());
    } else if(type == OBVCore::BibleQuoteDictModule) {
        ret = QSharedPointer<DictionaryModule>(new BibleQuoteDict());
    } else if(type == OBVCore::WebDictionaryModule) {
        ret = QSharedPointer<DictionaryModule>(new WebDictionary());
    }
    m_dictionaryModule = ret.toWeakRef();
    return ret;
}
