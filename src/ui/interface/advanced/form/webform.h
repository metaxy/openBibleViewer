#ifndef WEBFORM_H
#define WEBFORM_H

#include <QWidget>

#include "form.h"
namespace Ui {
    class WebForm;
}

class WebForm :  public Form
{
    Q_OBJECT

public:
    explicit WebForm(QWidget *parent = 0);
    ~WebForm();
    void init();

private:
    Ui::WebForm *ui;

public slots:
    void copy();
    void selectAll();
    void print();
    void printPreview();
    void saveFile();
    QString selectedText();

    void zoomIn();
    void zoomOut();
    void activated();
private slots:
    void loadPage();

};

#endif // WEBFORM_H
