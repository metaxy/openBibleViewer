#ifndef CONSOLEFORM_H
#define CONSOLEFORM_H

#include <QWidget>
#include "src/ui/interface/advanced/form/webviewform.h"

namespace Ui {
class ConsoleForm;
}


class ConsoleForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit ConsoleForm(QWidget *parent = 0);
    ~ConsoleForm();

    void init();
    void restore(const QString &key);
    void save();
    Form::FormType type() const;

private slots:
    void attach();
    void showContextMenu(QContextMenuEvent* ev);
private:
    Ui::ConsoleForm *ui;
};

#endif // CONSOLEFORM_H
