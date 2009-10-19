#ifndef MODULEDIALOG_H
#define MODULEDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
class moduleDialog;
}

class moduleDialog : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(moduleDialog)
public:
    explicit moduleDialog(QWidget *parent = 0);
    virtual ~moduleDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::moduleDialog *m_ui;
};

#endif // MODULEDIALOG_H
