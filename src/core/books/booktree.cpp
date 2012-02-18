#include "booktree.h"

BookTree::BookTree(const int i, const QString &t) : id(i), title(t), isRoot(false)
{
}
BookTree::BookTree() : id(-1), title(""), isRoot(true)
{
}
BookTree::~BookTree()
{
    foreach(BookTree* item, children)
    {
        delete item;
    }
    children.clear();
}
BookTree* BookTree::addChildren(const int id, const QString &title)
{
    BookTree *ret = new BookTree(id, title);
    children.append(ret);
    return ret;
}
