#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QObject>
#include <QtCore/QHash>
#include "src/core/basicclass.h"
#include "src/core/verseselection.h"
#include "src/ui/dock/notesdockwidget.h"
class NotesManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit NotesManager(QObject *parent = 0);
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
    void openNote(const QString &noteID);

signals:

public slots:

    void newCustomColorMark(VerseSelection selection, QColor color);
    void newBoldMark(VerseSelection selection);
    void newItalicMark(VerseSelection selection);
    void newUnderlineMark(VerseSelection selection);

    void removeMark(VerseSelection selection);
    void newNoteWithLink(VerseSelection selection);
private:
     QWidget *m_p;
    NotesDockWidget *m_notesDockWidget;
};

#endif // NOTESMANAGER_H
