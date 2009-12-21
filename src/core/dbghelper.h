#include <QtCore/QtDebug>
#define myDebug() qDebug() << Q_FUNC_INFO << " "

#ifndef QT_NO_DEBUG
#define DEBUG_FUNC_NAME DbgHelper dbgHelper(Q_FUNC_INFO);
#else
#define DEBUG_FUNC_NAME DbgHelper dbgHelper(Q_FUNC_INFO);
#endif
#ifndef DBGHELPER_H
#define DBGHELPER_H
#include <QtCore/QString>
/*!
 DbgHelper is a debug helper, it show the debug ouput more clearly

 @author Paul Walger <metaxy@walger.name>
*/
class DbgHelper
{
public:
    DbgHelper(const QString &t);
    ~DbgHelper();
private:
    QString txt;
    static int indent;
    static int colorIndex;
    int myColor;
};
#endif
