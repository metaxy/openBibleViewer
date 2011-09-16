#include "webdictionary.h"
#include <QtScript/QScriptEngine>

WebDictionary::WebDictionary()
{
    QScriptEngine myEngine;
    QScriptValue three = myEngine.evaluate("1 + 2");
}

QString WebDictionary::getEntry(const QString &entry)
{

}

QStringList WebDictionary::getAllKeys()
{

}
