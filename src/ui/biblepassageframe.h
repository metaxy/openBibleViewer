#ifndef BIBLEPASSAGEFRAME_H
#define BIBLEPASSAGEFRAME_H

#include <QFrame>
#include "src/core/basicclass.h"
namespace Ui {
    class BiblePassageFrame;
}

class BiblePassageFrame : public QFrame, public BasicClass
{
    Q_OBJECT

public:
    explicit BiblePassageFrame(QWidget *parent = 0);
    ~BiblePassageFrame();

private:
    Ui::BiblePassageFrame *ui;
};

#endif // BIBLEPASSAGEFRAME_H
