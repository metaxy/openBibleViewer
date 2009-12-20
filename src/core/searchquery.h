#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H
#include <QtCore/QString>
class SearchQuery
{
public:
    SearchQuery();
    QString searchText;
    bool regExp;
    bool wholeWord;
    bool caseSensitive;
};

#endif // SEARCHQUERY_H
