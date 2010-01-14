#ifndef STUDYINTERFACE_H
#define STUDYINTERFACE_H

#include <QWidget>
#include "src/ui/interface/interface.h"
namespace Ui
{
class StudyInterface;
}

class StudyInterface : public Interface
{
    Q_OBJECT
public:
    StudyInterface(QWidget *parent = 0);
    ~StudyInterface();
    void init();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StudyInterface *ui;
};

#endif // STUDYINTERFACE_H
