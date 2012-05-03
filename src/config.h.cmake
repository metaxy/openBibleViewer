#ifndef CONFIG_H
#define CONFIG_H


#include <QtCore/QStringList>

#define _AV_LANG "en;de;ru;cs;tr;sk;it"
#define _AV_LANG_NAME "English;German;Russian;Czech;Turkish;Slovak;Italian"
#define _AV_LANG_NAME_NATIVE "English;Deutsch;Русский;Česky;Türkmençe;Slovenčina;Italiano"
 
#cmakedefine OBV_PORTABLE_VERSION
#cmakedefine OBV_SWORD_SUPPORT
#cmakedefine OBV_USE_WSTRING
#cmakedefine OBV_NO_DEBUG_OUTPUT


#cmakedefine OBV_VERSION_NUMBER "@OBV_VERSION_NUMBER@"
#cmakedefine OBV_VERSION_STATUS "@OBV_VERSION_STATUS@"
#cmakedefine OBV_BUILD_DATE "@OBV_BUILD_DATE@"

#define QT_USE_QSTRINGBUILDER

#endif

