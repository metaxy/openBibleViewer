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

private:
    Ui::WebForm *ui;
};

#endif // WEBFORM_H
