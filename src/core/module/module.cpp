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
#include "src/core/module/module.h"
#include "src/core/module/bible/biblequote.h"
#include "src/core/module/bible/zefania-bible.h"
#include "src/core/module/bible/thewordbible.h"
#include "src/core/module/bible/swordbible.h"
#include "src/core/module/dictionary/biblequote-dict.h"
#include "src/core/module/dictionary/zefania-lex.h"
#include "src/core/module/dictionary/webdictionary.h"
#include "src/core/module/dictionary/swordlexicon.h"
#include "src/core/module/dictionary/theworddictionary.h"
#include "src/core/module/book/txtbook.h"
#include "src/core/module/commentary/webcommentary.h"
#include "src/core/module/commentary/thewordcommentary.h"
#include "src/core/module/commentary/eswordcommentary.h"
#include "src/core/module/book/thewordtopic.h"
#include "src/core/module/book/eswordtopic.h"
#include "src/core/module/book/rtfbook.h"
#include "src/core/module/book/pdfbook.h"
Module::Module(Module *parent) : m_parent(parent)
{
    m_moduleClass = ModuleTools::NoneClass;
    m_moduleType = ModuleTools::NoneType;

    if(m_parent != NULL) {
        m_parent->append(this);
    }
}
Module::~Module()
{
    m_bibleModule.clear();
    m_dictionaryModule.clear();
    m_bookModule.clear();
    m_commentaryModule.clear();
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
ModuleTools::ModuleClass Module::moduleClass() const
{
    return m_moduleClass;
}
ModuleTools::ModuleType Module::moduleType() const
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

void Module::setModuleClass(const ModuleTools::ModuleClass &c)
{
    m_moduleClass = c;
}

void Module::setModuleType(const ModuleTools::ModuleType &t)
{
    m_moduleType = t;
}

QSharedPointer<BibleModule> Module::newBibleModule(const ModuleTools::ModuleType type)
{
    QSharedPointer<BibleModule> ret;
    if(type == ModuleTools::ZefaniaBibleModule) {
        ret = QSharedPointer<BibleModule>(new ZefaniaBible());
    } else if(type == ModuleTools::TheWordBibleModule) {
        ret = QSharedPointer<BibleModule>(new TheWordBible());
    } else if(type == ModuleTools::SwordBibleModule) {
        ret = QSharedPointer<BibleModule>(new SwordBible());
    } else if(type == ModuleTools::BibleQuoteModule) {
        ret = QSharedPointer<BibleModule>(new BibleQuote());
    } else {
        myWarning() << "invalid type";
    }
    m_bibleModule = ret.toWeakRef();
    return ret;
}
QSharedPointer<DictionaryModule> Module::newDictionaryModule(const ModuleTools::ModuleType type)
{
    QSharedPointer<DictionaryModule> ret;
    if(type == ModuleTools::ZefaniaLexModule) {
        ret = QSharedPointer<DictionaryModule>(new ZefaniaLex());
    } else if(type == ModuleTools::BibleQuoteDictModule) {
        ret = QSharedPointer<DictionaryModule>(new BibleQuoteDict());
    } else if(type == ModuleTools::WebDictionaryModule) {
        ret = QSharedPointer<DictionaryModule>(new WebDictionary());
    } else if(type == ModuleTools::SwordLexiconModule) {
        ret = QSharedPointer<DictionaryModule>(new SwordLexicon());
    } else if(type == ModuleTools::TheWordDictionaryModule) {
        ret = QSharedPointer<DictionaryModule>(new TheWordDictionary());
    } else {
        myWarning() << "invalid type" << type;
    }
    m_dictionaryModule = ret.toWeakRef();
    return ret;
}
QSharedPointer<BookModule> Module::newBookModule(const ModuleTools::ModuleType type)
{
    QSharedPointer<BookModule> ret;
    if(type == ModuleTools::TxtBookModule) {
        ret = QSharedPointer<TxtBook>(new TxtBook());
    } else if(type == ModuleTools::RTFBookModule) {
        ret = QSharedPointer<RTFBook>(new RTFBook());
    } else if(type == ModuleTools::PDFBookModule) {
        ret = QSharedPointer<PDFBook>(new PDFBook());
    } else {
        myWarning() << "invalid type" << type;
    }
    m_bookModule = ret.toWeakRef();
    return ret;
}
QSharedPointer<CommentaryModule> Module::newCommentaryModule(const ModuleTools::ModuleType type)
{
    QSharedPointer<CommentaryModule> ret;
    if(type == ModuleTools::WebCommentaryModule) {
        ret = QSharedPointer<WebCommentary>(new WebCommentary());
    } else if(type == ModuleTools::TheWordCommentaryModule) {
        ret = QSharedPointer<TheWordCommentary>(new TheWordCommentary());
    } else if(type == ModuleTools::ESwordCommentaryModule) {
        ret = QSharedPointer<ESwordCommentary>(new ESwordCommentary());
    } else {
        myWarning() << "invalid type" << type;
    }
    m_commentaryModule = ret.toWeakRef();
    return ret;
}
QSharedPointer<TreeBookModule> Module::newTreeBookModule(const ModuleTools::ModuleType type)
{
    QSharedPointer<TreeBookModule> ret;
    if(type == ModuleTools::TheWordTopicModule) {
        ret = QSharedPointer<TheWordTopic>(new TheWordTopic());
    } else if(type == ModuleTools::ESwordTopicModule) {
        ret = QSharedPointer<ESwordTopic>(new ESwordTopic());
    } else {
        myWarning() << "invalid type";
    }
    m_treeBookModule = ret.toWeakRef();
    return ret;
}

SimpleModule* Module::newSimpleModule(const ModuleTools::ModuleType type)
{
    switch(type) {
    case ModuleTools::ZefaniaBibleModule:
        return new ZefaniaBible();
    case ModuleTools::BibleQuoteModule:
        return new BibleQuote();
    case ModuleTools::TheWordBibleModule:
        return new TheWordBible();
    case ModuleTools::SwordBibleModule:
        return new SwordBible();
    case ModuleTools::ESwordTopicModule:
        return new ESwordTopic();
    case ModuleTools::RTFBookModule:
        return new RTFBook();
    case ModuleTools::TheWordTopicModule:
        return new TheWordTopic();
    case ModuleTools::TxtBookModule:
        return new TxtBook();
    case ModuleTools::TheWordCommentaryModule:
        return new TheWordCommentary();
    case ModuleTools::ESwordCommentaryModule:
        return new ESwordCommentary();
    case ModuleTools::WebCommentaryModule:
        return new WebCommentary();
    case ModuleTools::BibleQuoteDictModule:
        return new BibleQuoteDict();
    case ModuleTools::SwordLexiconModule:
        return new SwordLexicon();
    case ModuleTools::WebDictionaryModule:
        return new WebDictionary();
    case ModuleTools::TheWordDictionaryModule:
        return new TheWordDictionary();
    case ModuleTools::ZefaniaLexModule:
        return new ZefaniaLex();
    case ModuleTools::PDFBookModule:
        return new ZefaniaLex();
    default:
        myWarning() << "could not create Module " << type;
        return NULL;

    }
}
