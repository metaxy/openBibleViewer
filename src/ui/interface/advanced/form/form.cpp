/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    m_id(-1),
    m_formRole(Form::NoRole),
    m_api(NULL)
{
}
Form::~Form()
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
void Form::setRole(const Form::FormRole role)
{
    m_formRole = role;
}
Form::FormRole Form::role() const
{
    return m_formRole;
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
    return *currentWindowID == m_id;
}

void Form::setParentSubWindow(MdiSubWindow *window)
{
    m_parentSubWindow = window;
}
SearchableModule * Form::searchableModule() const
{
    myWarning() << "calling Form::searchableModule()";
    return NULL;
}

