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
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant> //!!!
#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
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
        QSqlQuery query1("select name,value from config", db);
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
    } else if(fileName.endsWith(".cmtx", Qt::CaseInsensitive)) {
        return ModuleTools::ESwordCommentaryModule;
    } else if(fileName.endsWith(".xml", Qt::CaseInsensitive)) {
        QFile data(fileName);
        if(data.open(QFile::ReadOnly)) {
            QString fileData = "";
            QTextStream in(&data);
            for(int i = 0; i < 100; i++)
                fileData += in.readLine();
            //myDebug() << "fileData = " << fileData;
            if(fileData.contains("XMLBIBLE", Qt::CaseInsensitive) /*&& !(fileData.contains("x-quran", Qt::CaseInsensitive) || // i cannot allow this
                    fileData.contains("x-cult", Qt::CaseInsensitive) ||
                    fileData.contains("x-mormon", Qt::CaseInsensitive))*/) {
                return ModuleTools::ZefaniaBibleModule;
            } else if(fileData.contains("<dictionary", Qt::CaseInsensitive)) {
                return ModuleTools::ZefaniaLexModule;
            } else {
				myWarning() << "a strange xml file and not a zefania xml";
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
        case ModuleTools::ESwordCommentaryModule:
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


/*
 * if maxLevel == -1, then it will scan all subfolders
 */
QStringList ModuleTools::scan(const QString &path, const int maxLevel, const int level)
{
    QStringList ret;
    QDir dir(path);
    const QFileInfoList list = dir.entryInfoList();
    foreach(const QFileInfo & info, list) {
        if(info.fileName() != ".." && info.fileName() != ".") {
            if(info.isDir()) {
                if(level <= maxLevel || maxLevel < 0)
					ret.append(scan(info.absoluteFilePath(), maxLevel, level + 1));
            } else {
                ret.append(info.absoluteFilePath());
            }
        }
    }
    return ret;
}

QStringList ModuleTools::encodings()
{
    const QStringList ret = {"Apple Roman", "Big5", "Big5-HKSCS", "EUC-JP", "EUC-KR", "GB18030-0", "IBM 850",
                            "IBM 866", "IBM 874", "ISO 2022-JP", "ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", 
                            "ISO 8859-5", "ISO 8859-6", "ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10", 
                            "ISO 8859-13", "ISO 8859-14", "ISO 8859-15", "ISO 8859-16", "Iscii-Bng", "Dev", "Gjr", 
                            "Knd", "Mlm", "Ori", "Pnj", "Tlg", "Tml", "JIS X 0201", "JIS X 0208", "KOI8-R", 
                            "KOI8-U", "MuleLao-1", "ROMAN8", "Shift-JIS", "TIS-620", "TSCII", "UTF-8", "UTF-16", 
                            "UTF-16BE", "UTF-16LE", "UTF-32", "UTF-32BE", "UTF-32LE", "Windows-1250", "Windows-1251", "Windows-1252", 
                            "Windows-1253", "Windows-1254", "Windows-1255", "Windows-1256", "Windows-1257", "Windows-1258", "WINSAMI2"};
    return ret;
}
QHash<QString,int> ModuleTools::eSwordMap()
{
    QHash<QString, int> ret;

    ret["Gen"] = 0;
    ret["Exo"] = 1;
    ret["Lev"] = 2;
    ret["Num"] = 3;
    ret["Deu"] = 4;
    ret["Jos"] = 5;
    ret["Jdg"] = 6;
    ret["Rth"] = 7;
    ret["1Sa"] = 8;
    ret["2Sa"] = 9;
    ret["1Ki"] = 10;
    ret["2Ki"] = 11;
    ret["1Ch"] = 12;
    ret["2Ch"] = 13;
    ret["Ezr"] = 14;
    ret["Neh"] = 15;
    ret["Est"] = 16;
    ret["Job"] = 17;
    ret["Psa"] = 18;
    ret["Spr"] = 19;
    ret["Son"] = 20;
    ret["Isa"] = 21;
    ret["Exo"] = 22;
    ret["Exo"] = 23;
    ret["Exo"] = 24;
    ret["Exo"] = 25;
    ret["Exo"] = 26;
    ret["Exo"] = 27;
    ret["Exo"] = 28;
    ret["Exo"] = 29;
    ret["Exo"] = 30;
    ret["Exo"] = 31;
    ret["Exo"] = 32;
    ret["Exo"] = 33;
    ret["Exo"] = 34;
    ret["Exo"] = 35;
    ret["Exo"] = 36;
    ret["Exo"] = 37;
    ret["Exo"] = 38;
    ret["Exo"] = 39;
    ret["Exo"] = 40;
    ret["Exo"] = 41;
    ret["Exo"] = 42;
    ret["Exo"] = 43;
    ret["Exo"] = 44;
    ret["Exo"] = 45;
    ret["Exo"] = 46;
    ret["Exo"] = 47;
    ret["Exo"] = 48;
    ret["Exo"] = 49;
    ret["Exo"] = 50;
    ret["Exo"] = 51;
    ret["Exo"] = 52;
    ret["Exo"] = 53;
    ret["Exo"] = 54;
    ret["Exo"] = 55;
    ret["Exo"] = 56;
    ret["Exo"] = 57;
    ret["Exo"] = 58;
    ret["Exo"] = 59;
    ret["Exo"] = 60;
    ret["Exo"] = 61;
    ret["Exo"] = 62;
    ret["Exo"] = 63;
    ret["Exo"] = 64;
    ret["Exo"] = 65;


    return ret;
}
