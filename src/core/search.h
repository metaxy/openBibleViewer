#ifndef SEARCH_H
#define SEARCH_H

#include <QtGui/QDialog>

namespace Ui
{
class search;
}

class search : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(search)
public:
    explicit search(QWidget *parent = 0);
    virtual ~search();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::search *m_ui;
};

#endif // SEARCH_H
