#ifndef BIBLELISTWIDGET_H
#define BIBLELISTWIDGET_H

#include <QtGui/QDialog>
#include "src/core/basicclass.h"
#include <QtGui/QStandardItemModel>
namespace Ui {
    class BibleListWidget;
}

class BibleListWidget : public QDialog, public BasicClass {
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

    void save();
private:
    Ui::BibleListWidget *ui;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selectionModel;
};

#endif // BIBLELISTWIDGET_H
