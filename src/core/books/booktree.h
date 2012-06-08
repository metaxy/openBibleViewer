#ifndef BOOKTREE_H
#define BOOKTREE_H
#include <QtCore/QList>
#include <QtCore/QString>
class BookTree
{
public:
    BookTree(const int id, const QString &t);
    BookTree();
    ~BookTree();
    int id() const;
    bool isRoot() const;
    QString title() const;
    QList<BookTree *> children() const;


    BookTree* addChildren(const int id, const QString &title);
private:
    int m_id;
    QString m_title;
    bool m_isRoot;
    QList<BookTree *> m_children;
};

#endif // BOOKTREE_H
