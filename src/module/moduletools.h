#ifndef MODULETOOLS_H
#define MODULETOOLS_H
#include <QtCore/QString>
class ModuleTools
{
public:
    ModuleTools();

    enum ModuleClass {
        NoneClass = 0,
        BibleModuleClass = 1,
        DictionaryModuleClass = 2,
        FolderClass = 3,
        WebPageClass = 4
    };

    enum ModuleType {
        NoneType = 0,
        BibleQuoteModule = 1,
        ZefaniaBibleModule = 2,
        ZefaniaLexModule = 3,
        BibleQuoteDictModule = 4,
        TheWordBibleModule = 5,
        FolderModule = 6,
        SwordBibleModule = 7,
        WebPageModule = 8,
        WebDictionaryModule = 9,
        SwordLexiconModule = 10
    };

    enum DefaultModule {
        NotADefaultModule = 0,
        DefaultBibleModule = 1,
        DefaultStrongDictModule = 2,
        DefaultGramDictModule = 3,
        DefaultDictModule = 4,
        DefaultRMACDictModule = 5
    };

    enum ContentType {
        UnkownContent = 0,
        BibleContent =  1,
        BibleOTContent = 2,
        BibleNTContent = 3,
        StrongsContent = 4,
        StrongsHebrewContent = 5,
        StrongsGreekContent = 6,
        RMacContent = 7,
        WordDictionaryContent = 8,
        DictionaryContent = 9,
        GramContent = 10

    };

    enum ModuleCategory {
        UnkownCategory = 0,
        FolderCategory = 1,
        BibleCategory = 2,
        DictionaryCategory = 3,
        CommentaryCategory = 4,
        BookCategory = 5
    };

    static const QString strongScheme;
    static const QString gramScheme;
    static const QString rmacScheme;
    static const QString dictScheme;


    static ModuleTools::ModuleType recognizeModuleType(const QString &fileName);
    static ModuleTools::DefaultModule toDefaultModule(const ModuleTools::ContentType t);
    static bool alsoOk(const ModuleTools::ContentType t1, const ModuleTools::ContentType t2);

    static ModuleTools::ModuleCategory getCategory(ModuleTools::ModuleType type);

    static QStringList moduleTypeNames();
    static QString moduleTypeName(ModuleTools::ModuleType type);

    static ModuleTools::ModuleClass typeToClass(ModuleTools::ModuleType type);

    static Qt::LayoutDirection languageToDirection(const QString &lang);
};

#endif // MODULETOOLS_H
