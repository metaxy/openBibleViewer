#ifndef VERSESELECTION_H
#define VERSESELECTION_H
#include <QtCore/QString>
class VerseSelection
{
public:
    VerseSelection();
    int startVerse;
    int endVerse;
    int posInStartVerse;
    int posInEndVerse;
    QString selectedText;
    QString shortestStringInStartVerse;
    QString shortestStringInEndVerse;


};

#endif // VERSESELECTION_H
