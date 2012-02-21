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
        WebPageClass = 4,
        BookClass = 5,
        CommentaryClass = 6,
        TreeBookClass = 7
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
        SwordLexiconModule = 10,
        TxtBookModule = 11,
        WebCommentaryModule = 12,
        TheWordCommentaryModule = 13,
        RTFBookModule = 14,
        TheWordTopicModule = 15,
        ESwordTopicModule = 16,
        TheWordDictionaryModule = 17
    };

    enum DefaultModule {
        NotADefaultModule = 0,
        DefaultBibleModule = 1,
        DefaultStrongDictModule = 2,
        DefaultGramDictModule = 3,
        DefaultDictModule = 4,
        DefaultRMACDictModule = 5,
        DefaultCommentary = 6
    };

    enum ContentType {
        UnkownContent = 0,
        BibleContent =  1,
        BibleOTContent = 2,
        BibleNTContent = 3,
        StrongsContent = 4,
        StrongsHebrewContent = 5,
        StrongsGreekContent = 6,
        RMACContent = 7,
        WordDictionaryContent = 8,
        DictionaryContent = 9,
        GramContent = 10,
        BookContent = 11,
        CommentaryContent = 12,
        TreeBookContent = 13

    };

    enum ModuleCategory {
        UnkownCategory = 0,
        FolderCategory = 1,
        BibleCategory = 2,
        DictionaryCategory = 3,
        CommentaryCategory = 4,
        BookCategory = 5,
        TreeBookCategory = 6
    };

    static const QString strongScheme;
    static const QString gramScheme;
    static const QString rmacScheme;
    static const QString dictScheme;
    static const QString verseScheme;
    static const QString webPageScheme;
    static const QString bookScheme;
    static const QString theWordScheme;
    static const QString userInputScheme;
    static const QString treeBookScheme;
    static const QString noteScheme;

    static ModuleTools::ModuleType recognizeModuleType(const QString &fileName);
    static ModuleTools::DefaultModule toDefaultModule(const ModuleTools::ContentType t);
    static bool alsoOk(const ModuleTools::ContentType t1, const ModuleTools::ContentType t2);

    static ModuleTools::ModuleCategory getCategory(ModuleTools::ModuleType type);

    static QStringList moduleTypeNames();
    static QString moduleTypeName(ModuleTools::ModuleType type);

    static ModuleTools::ModuleClass typeToClass(ModuleTools::ModuleType type);

    static Qt::LayoutDirection languageToDirection(const QString &lang);

    static ModuleTools::ContentType contentTypeFromUrl(const QString &url);
    static ModuleTools::ModuleClass moduleClassFromUrl(const QString &url);

};

#endif // MODULETOOLS_H
