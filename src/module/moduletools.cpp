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
#include "moduletools.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "src/core/dbghelper.h"
const QString ModuleTools::strongScheme = "strong:/";
const QString ModuleTools::gramScheme = "gram:/";
const QString ModuleTools::rmacScheme = "rmac:/";
const QString ModuleTools::dictScheme = "dict:/";
const QString ModuleTools::verseScheme = "verse:/";
const QString ModuleTools::webPageScheme = "webpage:/";
const QString ModuleTools::bookScheme = "book:/";
const QString ModuleTools::theWordScheme = "tw://";
const QString ModuleTools::userInputScheme = "uinput:/";
const QString ModuleTools::treeBookScheme = "tbook:/";
const QString ModuleTools::noteScheme = "note:/";
ModuleTools::ModuleTools()
{
}

ModuleTools::ModuleType ModuleTools::recognizeModuleType(const QString &fileName)
{
    //myDebug() << fileName;
    if(fileName.endsWith("bibleqt.ini", Qt::CaseInsensitive)) {
        return ModuleTools::BibleQuoteModule;
    } else if(fileName.endsWith(".webdict.xml", Qt::CaseInsensitive)) {
        return ModuleTools::WebDictionaryModule;
    } else if(fileName.endsWith(".webpage.xml", Qt::CaseInsensitive)) {
        return ModuleTools::WebPageModule;
    } else if(fileName.endsWith(".webcom.xml", Qt::CaseInsensitive)) {
        return ModuleTools::WebCommentaryModule;
    } else if(fileName.endsWith(".twm", Qt::CaseInsensitive)) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(fileName);
        if (!db.open()) {
            myWarning() << "could not open database " << fileName;
            return ModuleTools::NoneType;
        }
        QSqlQuery query1 ("select name,value from config", db);
        while (query1.next()) {
            const QString name = query1.value(0).toString();
            if(name == "type") {
                 const int value = query1.value(1).toInt();
                 if(value == 1) {
                     return ModuleTools::TheWordDictionaryModule;
                 } else if(value == 2) {
                     return ModuleTools::TheWordCommentaryModule;
                 } else if(value == 3) {
                     return ModuleTools::TheWordTopicModule;
                 } else {
                     return ModuleTools::NoneType;
                 }
            }
        }
        db.close();
    } else if(fileName.endsWith(".topx", Qt::CaseInsensitive)) {
        return ModuleTools::ESwordTopicModule;
    } else if(fileName.endsWith(".xml", Qt::CaseInsensitive)) {
        QFile data(fileName);
        if(data.open(QFile::ReadOnly)) {
            QString fileData = "";
            QTextStream in(&data);
            for(int i = 0; i < 100; i++)
                fileData += in.readLine();
            //myDebug() << "fileData = " << fileData;
            if(fileData.contains("XMLBIBLE", Qt::CaseInsensitive) && !(fileData.contains("x-quran", Qt::CaseInsensitive) || // i cannot allow this
                    fileData.contains("x-cult", Qt::CaseInsensitive) ||
                    fileData.contains("x-mormon", Qt::CaseInsensitive))) {
                return ModuleTools::ZefaniaBibleModule;
            } else if(fileData.contains("<dictionary", Qt::CaseInsensitive)) {
                return ModuleTools::ZefaniaLexModule;
            }

        } else {
            myWarning() << "could not open file " << fileName;
        }
    } else if(fileName.endsWith(".idx", Qt::CaseInsensitive)) {
        return ModuleTools::BibleQuoteDictModule;
    } else if(fileName.endsWith(".nt", Qt::CaseInsensitive) || fileName.endsWith(".ot", Qt::CaseInsensitive) || fileName.endsWith(".ont", Qt::CaseInsensitive)) {
        return ModuleTools::TheWordBibleModule;
    } else if(fileName.endsWith(".txt", Qt::CaseInsensitive)) {
        return ModuleTools::TxtBookModule;
    } else if(fileName.endsWith(".rtf", Qt::CaseInsensitive)) {
        return ModuleTools::RTFBookModule;
    }
    return ModuleTools::NoneType;
}

ModuleTools::DefaultModule ModuleTools::toDefaultModule(const ModuleTools::ContentType t)
{
    switch (t) {
        case ModuleTools::BibleContent:
        case ModuleTools::BibleOTContent:
        case ModuleTools::BibleNTContent:
            return ModuleTools::DefaultBibleModule;
        case ModuleTools::StrongsContent:
        case ModuleTools::StrongsGreekContent:
        case ModuleTools::StrongsHebrewContent:
            return ModuleTools::DefaultStrongDictModule;
        case ModuleTools::RMACContent:
            return ModuleTools::DefaultRMACDictModule;
        case ModuleTools::WordDictionaryContent:
            return ModuleTools::DefaultDictModule;
        case ModuleTools::GramContent:
            return ModuleTools::DefaultGramDictModule;
        default:
            return ModuleTools::NotADefaultModule;//todo: could this cause some bugs?
    }
}
/**
 * t2 is the content we want.
 */
bool ModuleTools::alsoOk(const ModuleTools::ContentType t1, const ModuleTools::ContentType t2)
{
    if(t1 == t2)
        return true;

    if(t2 == ModuleTools::BibleOTContent && t1 == ModuleTools::BibleContent) {
            return true;
    }
    if(t2 == ModuleTools::BibleNTContent && t1 == ModuleTools::BibleContent) {
            return true;
    }

    if(t2 == ModuleTools::StrongsGreekContent && t1 == ModuleTools::StrongsContent) {
            return true;
    }
    if(t2 == ModuleTools::StrongsHebrewContent && t1 == ModuleTools::StrongsContent) {
            return true;
    }
    return false;

}
QStringList ModuleTools::moduleTypeNames()
{

    QStringList l;
    l << QT_TRANSLATE_NOOP("Core", "None") << QT_TRANSLATE_NOOP("Core", "BibleQuote") << QT_TRANSLATE_NOOP("Core", "Zefania XML Bible")
      << QT_TRANSLATE_NOOP("Core", "Zefania Lex Module") << QT_TRANSLATE_NOOP("Core", "BibleQuote Dictionary") << QT_TRANSLATE_NOOP("Core", "The Word Bible")
      << QT_TRANSLATE_NOOP("Core", "Folder")
     << QT_TRANSLATE_NOOP("Core", "Sword Bible") << QT_TRANSLATE_NOOP("Core", "Web Page") << QT_TRANSLATE_NOOP("Core", "Web Dictionary")
     << QT_TRANSLATE_NOOP("Core", "Sword Lexicon") << QT_TRANSLATE_NOOP("Core", "*.txt Book")
     << QT_TRANSLATE_NOOP("Core", "Web Commentary") << QT_TRANSLATE_NOOP("Core", "The Word Commentary") << QT_TRANSLATE_NOOP("Core", "*.rtf Book")
     << QT_TRANSLATE_NOOP("Core", "TheWord Topic File") << QT_TRANSLATE_NOOP("Core", "e-Sword Topic File") << QT_TRANSLATE_NOOP("Core", "TheWord Dictionary");
    return l;
}

QString ModuleTools::moduleTypeName(ModuleTools::ModuleType type)
{
    if(type == ModuleTools::NoneType) {
        return QT_TRANSLATE_NOOP("Core", "None");
    } else if(type == ModuleTools::BibleQuoteModule) {
        return QT_TRANSLATE_NOOP("Core", "BibleQuote");
    } else if(type == ModuleTools::ZefaniaBibleModule) {
        return QT_TRANSLATE_NOOP("Core", "Zefania XML Bible");
    } else if(type == ModuleTools::ZefaniaLexModule) {
        return QT_TRANSLATE_NOOP("Core", "Zefania Lex Module");
    } else if(type == ModuleTools::BibleQuoteDictModule) {
        return QT_TRANSLATE_NOOP("Core", "BibleQuote Dictionary");
    } else if(type == ModuleTools::TheWordBibleModule) {
        return QT_TRANSLATE_NOOP("Core", "The Word Bible");
    } else if(type == ModuleTools::SwordBibleModule) {
        return QT_TRANSLATE_NOOP("Core", "Sword Bible");
    } else if(type == ModuleTools::WebPageModule) {
        return QT_TRANSLATE_NOOP("Core", "Web Page");
    } else if(type == ModuleTools::WebDictionaryModule) {
        return QT_TRANSLATE_NOOP("Core", "Web Dictionary");
    } else if(type == ModuleTools::FolderModule) {
        return QT_TRANSLATE_NOOP("Core", "Folder");
    } else if(type == ModuleTools::SwordLexiconModule) {
        return QT_TRANSLATE_NOOP("Core", "Sword Lexicon");
    }  else if(type == ModuleTools::TxtBookModule) {
        return QT_TRANSLATE_NOOP("Core", "Txt Book");
    } else if(type == ModuleTools::WebCommentaryModule) {
        return QT_TRANSLATE_NOOP("Core", "Web Commentary");
    } else if(type == ModuleTools::TheWordCommentaryModule) {
        return QT_TRANSLATE_NOOP("Core", "The Word Commentary");
    } else if(type == ModuleTools::RTFBookModule) {
        return QT_TRANSLATE_NOOP("Core", "*.rtf Book");
    } else if(type == ModuleTools::TheWordTopicModule) {
        return QT_TRANSLATE_NOOP("Core", "TheWord Topic Module");
    } else if(type == ModuleTools::ESwordTopicModule) {
        return QT_TRANSLATE_NOOP("Core", "e-Sword Topic File");
    } else if(type == ModuleTools::TheWordDictionaryModule) {
        return QT_TRANSLATE_NOOP("Core", "TheWord Dictionary");
    }
    return "";
}
ModuleTools::ModuleCategory ModuleTools::getCategory(ModuleTools::ModuleType type)
{
    switch (type) {
        case ModuleTools::BibleQuoteModule:
        case ModuleTools::ZefaniaBibleModule:
        case ModuleTools::TheWordBibleModule:
        case ModuleTools::SwordBibleModule:
            return ModuleTools::BibleCategory;

        case ModuleTools::ZefaniaLexModule:
        case ModuleTools::BibleQuoteDictModule:
        case ModuleTools::WebDictionaryModule:
        case ModuleTools::SwordLexiconModule:
        case ModuleTools::TheWordDictionaryModule:
            return ModuleTools::DictionaryCategory;

        case ModuleTools::FolderModule:
            return ModuleTools::FolderCategory;

        case ModuleTools::WebPageModule:
            return ModuleTools::BookCategory;

        case ModuleTools::TxtBookModule:
        case ModuleTools::RTFBookModule:
            return ModuleTools::BookCategory;

        case ModuleTools::TheWordTopicModule:
        case ModuleTools::ESwordTopicModule:
            return ModuleTools::TreeBookCategory;

        case ModuleTools::WebCommentaryModule:
        case ModuleTools::TheWordCommentaryModule:
            return ModuleTools::CommentaryCategory;
        default:
            return ModuleTools::UnkownCategory;
    }
}
ModuleTools::ModuleClass ModuleTools::typeToClass(ModuleTools::ModuleType type)
{
    switch(type)
    {
        case ModuleTools::BibleQuoteModule:
        case ModuleTools::ZefaniaBibleModule:
        case ModuleTools::TheWordBibleModule:
        case ModuleTools::SwordBibleModule:
            return ModuleTools::BibleModuleClass;

        case ModuleTools::ZefaniaLexModule:
        case ModuleTools::BibleQuoteDictModule:
        case ModuleTools::WebDictionaryModule:
        case ModuleTools::SwordLexiconModule:
        case ModuleTools::TheWordDictionaryModule:
            return ModuleTools::DictionaryModuleClass;

        case ModuleTools::WebPageModule:
            return ModuleTools::WebPageClass;

        case ModuleTools::TxtBookModule:
        case ModuleTools::RTFBookModule:
            return ModuleTools::BookClass;

        case ModuleTools::TheWordTopicModule:
        case ModuleTools::ESwordTopicModule:
            return ModuleTools::TreeBookClass;

        case ModuleTools::WebCommentaryModule:
        case ModuleTools::TheWordCommentaryModule:
            return ModuleTools::CommentaryClass;

        default:
            return ModuleTools::NoneClass;
    }
    return ModuleTools::NoneClass;
}
Qt::LayoutDirection ModuleTools::languageToDirection(const QString &lang)
{
    if(lang.toLower() == "heb") {
        return Qt::RightToLeft;
    }
    return Qt::LeftToRight;
}
ModuleTools::ContentType ModuleTools::contentTypeFromUrl(const QString &url)
{
    if(url.startsWith(ModuleTools::strongScheme)) {
        return ModuleTools::StrongsContent;
    } else if(url.startsWith(ModuleTools::rmacScheme)) {
        return ModuleTools::RMACContent;
    } else if(url.startsWith(ModuleTools::verseScheme)) {
        return ModuleTools::BibleContent;
    } else if(url.startsWith(ModuleTools::theWordScheme) && url.contains("bible")) {
        return ModuleTools::BibleContent;
    }
    return ModuleTools::UnkownContent;
}
ModuleTools::ModuleClass ModuleTools::moduleClassFromUrl(const QString &url)
{
    if(url.startsWith(ModuleTools::strongScheme)) {
        return ModuleTools::DictionaryModuleClass;
    } else if(url.startsWith(ModuleTools::rmacScheme)) {
        return ModuleTools::DictionaryModuleClass;
    } else if(url.startsWith(ModuleTools::verseScheme)) {
        return ModuleTools::BibleModuleClass;
    } else if(url.startsWith(ModuleTools::theWordScheme) && url.contains("bible")) {
        return ModuleTools::BibleModuleClass;
    }
    return ModuleTools::NoneClass;
}


