#ifndef MARKLIST_H
#define MARKLIST_H

#include <QDialog>

namespace Ui {
    class MarkList;
}

class MarkList : public QDialog {
    Q_OBJECT
public:
    MarkList(QWidget *parent = 0);
    ~MarkList();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MarkList *ui;
};

#endif // MARKLIST_H
