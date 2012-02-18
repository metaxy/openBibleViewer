#ifndef BOOKTREE_H
#define BOOKTREE_H
#include <QList>
#include <QString>
class BookTree
{
public:
    BookTree(const int id, const QString &t);
    BookTree();
    ~BookTree();
    int id;
    QString title;
    bool isRoot;
    QList<BookTree *> children;

    BookTree* addChildren(const int id, const QString &title);

};

#endif // BOOKTREE_H
