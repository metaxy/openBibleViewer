#ifndef MARKSDOCKWIDGET_H
#define MARKSDOCKWIDGET_H

#include <QWidget>

namespace Ui {
class MarksDockWidget;
}

class MarksDockWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MarksDockWidget(QWidget *parent = 0);
    ~MarksDockWidget();
    
private:
    Ui::MarksDockWidget *ui;
};

#endif // MARKSDOCKWIDGET_H
