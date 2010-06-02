#ifndef BIBLELISTWIDGET_H
#define BIBLELISTWIDGET_H

#include <QWidget>
#include "src/core/basicclass.h"
namespace Ui {
    class BibleListWidget;
}

class BibleListWidget : public QWidget, public BasicClass {
    Q_OBJECT
public:
    explicit BibleListWidget(QWidget *parent = 0);
    ~BibleListWidget();
    void init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BibleListWidget *ui;
};

#endif // BIBLELISTWIDGET_H
