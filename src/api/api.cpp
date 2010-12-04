#include "api.h"

Api::Api(QObject *parent) :
    QObject(parent)
{
}
void Api::init()
{
    m_bibleApi = new BibleApi();
    setAll(m_bibleApi);
    m_notesApi = new NotesApi();
    setAll(m_notesApi);
}
BibleApi* Api::bibleApi() const
{
    return m_bibleApi;
}
NotesApi* Api::notesApi() const
{
    return m_notesApi;
}
