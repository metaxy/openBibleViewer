#include "dbghelper.h"
#define NO_COLOR
#define DBGHELPER_USES_PRINTF
#ifdef DBGHELPER_USES_PRINTF
#include <stdio.h>
#else
#include <QtCore/QtDebug>
#endif

int DbgHelper::indent = 0;
int DbgHelper::colorIndex = 0;


static void DbgHelper_output(int color, int indent,
                             const QString &prefix, const QString &funcName)
{
    QString text = QString(4 * indent, ' ') + QString(prefix + " " + funcName);
    if (color >= 0) {
        text.prepend("\x1b[3" + QString::number(1 + color) + "m");
        text.append("\x1b[39m");
    }
#ifndef DBGHELPER_USES_PRINTF
    qDebug() << text;
#else
    fprintf(stderr, "%s\n", qPrintable(text));
#endif
}

DbgHelper::DbgHelper(const QString &t)
{
    //qDebug() << "DbgHelper::DebgHelper";
    txt = t;
#ifdef NO_COLOR
    myColor = -1;
#else
    myColor = colorIndex;
    colorIndex = (colorIndex + 1) % 7;
#endif
    DbgHelper_output(myColor, indent, "BEGIN", txt);
    ++indent;
}

DbgHelper::~DbgHelper()
{
    --indent;
    DbgHelper_output(myColor, indent, "END", txt);
}
