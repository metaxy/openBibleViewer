#include "actions.h"

Actions::Actions(QObject *parent) :
    QObject(parent)
{
}
void Actions::showChapter(const int &moduleID, const int &bookID, const int &chapterID)
{
    emit _showChapter(moduleID, bookID, chapterID);
}
void Actions::get(const QString &url)
{
    emit _get(url);
}
void Actions::get(const QUrl &url)
{
    emit _get(url.toString());
}
void Actions::get(const BibleUrl &url)
{
    emit _get(url);
}
void Actions::showNote(const QString &noteID)
{
    emit _showNote(noteID);
}
