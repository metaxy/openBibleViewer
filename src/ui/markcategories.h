#ifndef MARKCATEGORIES_H
#define MARKCATEGORIES_H

#include <QDialog>

namespace Ui {
    class MarkCategories;
}

class MarkCategories : public QDialog {
    Q_OBJECT
public:
    MarkCategories(QWidget *parent = 0);
    ~MarkCategories();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MarkCategories *ui;
};

#endif // MARKCATEGORIES_H
