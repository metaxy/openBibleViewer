#ifndef BIBLELISTWIDGET_H
#define BIBLELISTWIDGET_H

#include <QWidget>
#include <QtGui/QFrame>
#include "src/core/basicclass.h"
#include <QtGui/QStandardItemModel>
namespace Ui {
    class BibleListWidget;
}

class BibleListWidget : public QFrame, public BasicClass {
    Q_OBJECT
public:
    explicit BibleListWidget(QWidget *parent = 0);
    ~BibleListWidget();
    void init();

protected:
    void changeEvent(QEvent *e);
private slots:
    void removeCol();
    void addCol();

    void removeRow();
    void addRow();
private:
    Ui::BibleListWidget *ui;
    QStandardItemModel *m_model;
};

#endif // BIBLELISTWIDGET_H
