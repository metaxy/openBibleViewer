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
#ifndef OBVCORE_H
#define OBVCORE_H
#include <QtCore/QStringList>
namespace OBVCore
{

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
    WebDictionaryModule = 9
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

static QString strongScheme = "strong:/";
static QString gramScheme = "gram:/";
static QString rmacScheme = "rmac:/";

typedef quint32 rid;

/*  QStringList ModuleTypeNames()
  {
      QStringList l;
    * l << QT_TRANSLATE_NOOP("Core", "None") << QT_TRANSLATE_NOOP("Core", "BibleQuote") << QT_TRANSLATE_NOOP("Core", "Zefania XML Bible")
        << QT_TRANSLATE_NOOP("Core", "Zefania Lex Module") << QT_TRANSLATE_NOOP("Core", "BibleQuote Dictionary") << QT_TRANSLATE_NOOP("Core", "The Word Bible");
      return l;
  }

  QString ModuleTypeName(ModuleType type)
  {
      if(type == NoneType) {
          return QObject::tr("None");
      } else if(type == BibleQuoteModule) {
          return QObject::tr("BibleQuote");
      } else if(type == ZefaniaBibleModule) {
          return QObject::tr("Zefania XML Bible");
      }else if(type == ZefaniaLexModule) {
          return QObject::tr("Zefania Lex Module");
      }else if(type == BibleQuoteDictModule) {
          return QObject::tr("BibleQuote Dictionary");
      }else if(type == TheWordBibleModule) {
          return QObject::tr("The Word Bible");
      }
      return "";
  }*/

}

#endif //OBVCORE_H
