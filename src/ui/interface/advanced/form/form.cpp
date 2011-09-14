#include "form.h"

Form::Form(QWidget *parent) :
    QWidget(parent)
{
}

void Form::setID(const int id)
{
    m_id = id;
}

int Form::id()
{
    return m_id;
}

void Form::setApi(Api *api)
{
    m_api = api;
}

void Form::setNotesManager(NotesManager *notesManager)
{
    m_notesManager = notesManager;
}

void Form::setBookmarksManager(BookmarksManager *bookmarksManager)
{
    m_bookmarksManager = bookmarksManager;
}

bool Form::active()
{
    if(*currentWindowID == m_id)
        return true;
    return false;
}

void Form::setParentSubWindow(QMdiSubWindow *window)
{
    m_parentSubWindow = window;
}

SearchableModule * Form::searchableModule()
{
    return NULL;
}
