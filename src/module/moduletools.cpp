#include "moduletools.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "src/core/dbghelper.h"
const QString ModuleTools::strongScheme = "strong:/";
const QString ModuleTools::gramScheme = "gram:/";
const QString ModuleTools::rmacScheme = "rmac:/";

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
        case ModuleTools::RMacContent:
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
     << QT_TRANSLATE_NOOP("Core", "Sword Lexicon");
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
    } else if(type == ModuleTools::SwordLexicon) {
        return QT_TRANSLATE_NOOP("Core", "Sword Lexicon");
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
            break;
        case ModuleTools::ZefaniaLexModule:
        case ModuleTools::BibleQuoteDictModule:
        case ModuleTools::WebDictionaryModule:
    case ModuleTools::SwordLexicon:
            return ModuleTools::DictionaryCategory;
            break;
        case ModuleTools::FolderModule:
            return ModuleTools::FolderCategory;
            break;
        case ModuleTools::WebPageModule:
            return ModuleTools::BookCategory;
            break;
    }


}
