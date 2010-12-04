#ifndef API_H
#define API_H

#include <QObject>
#include "src/core/basicclass.h"
#include "bibleapi.h"
#include "notesapi.h"
class Api : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit Api(QObject *parent = 0);
    void init();
    NotesApi* notesApi() const;
    BibleApi* bibleApi() const;
signals:

public slots:
private:
    NotesApi *m_notesApi;
    BibleApi *m_bibleApi;
};

#endif // API_H
