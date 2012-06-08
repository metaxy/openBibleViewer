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
