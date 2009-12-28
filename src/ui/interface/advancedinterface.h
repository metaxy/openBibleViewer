#ifndef ADVANCEDINTERFACE_H
#define ADVANCEDINTERFACE_H

#include <QWidget>
#include "src/ui/interface/interface.h"
namespace Ui
{
class AdvancedInterface;
}

class AdvancedInterface : public Interface
{
    Q_OBJECT
public:
    AdvancedInterface(QWidget *parent = 0);
    ~AdvancedInterface();
    void init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AdvancedInterface *ui;
};

#endif // ADVANCEDINTERFACE_H
