#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H
#include <QtCore/QString>
struct searchQuery {
    QString text;
    bool regexp;
    bool whole;
    bool caseSensitive;
};

#endif // SEARCHQUERY_H
