#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QObject>
#include "src/core/basicclass.h"
class NotesManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit NotesManager(QObject *parent = 0);

signals:

public slots:

};

#endif // NOTESMANAGER_H
