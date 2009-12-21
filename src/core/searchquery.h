#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H
#include <QtCore/QString>
/*!
 SearchQuery represent a search query

 @author Paul Walger <metaxy@walger.name>
*/
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
