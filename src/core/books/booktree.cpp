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
#include "booktree.h"

BookTree::BookTree(const int i, const QString &t) : m_id(i), m_title(t), m_isRoot(false)
{
}
BookTree::BookTree() : m_id(-1), m_title(""), m_isRoot(true)
{
}
BookTree::~BookTree()
{
    foreach(BookTree* item, m_children)
    {
        delete item;
    }
    m_children.clear();
}
BookTree* BookTree::addChildren(const int id, const QString &title)
{
    BookTree *ret = new BookTree(id, title);
    m_children.append(ret);
    return ret;
}
int BookTree::id() const
{
    return m_id;
}
bool BookTree::isRoot() const
{
    return m_isRoot;
}
QString BookTree::title() const
{
    return m_title;
}
QList<BookTree *> BookTree::children() const
{
    return m_children;
}
