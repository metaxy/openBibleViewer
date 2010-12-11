#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include "src/core/bible/bibleurl.h"
class Actions : public QObject
{
    Q_OBJECT
public:
    explicit Actions(QObject *parent = 0);

signals:
    void _showChapter(const int &moduleID, const int &bookID, const int &chapterID);
    /**
      By default the Interface pharse all urls.
      */
    void _get(const QString &url);
    void _get(const BibleUrl &url);

    void _showNote(const QString &nodeID);

public slots:
    /**
      Show a chapter in current SubWindow.
      */
    void showChapter(const int &moduleID, const int &bookID, const int &chapterID);

    void get(const QString &url);
    void get(const QUrl &url);
    void get(const BibleUrl &url);

    void showNote(const QString &noteID);

};

#endif // ACTIONS_H
