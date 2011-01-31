#ifndef BIBLEPASSAGEFRAME_H
#define BIBLEPASSAGEFRAME_H

#include <QFrame>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include "src/core/basicclass.h"
#include "src/core/verse/verseurl.h"
namespace Ui {
    class BiblePassageFrame;
}

class BiblePassageFrame : public QFrame, public BasicClass
{
    Q_OBJECT
private slots:
    void addVerse(const int bookID = 0, const int chapterID = 0, const int verseID = 0);
    void addMVerse(const int bookID = 0, const int chapterID = 0, const int startVerseID = 0, const int endVerseID=0);

public:
    explicit BiblePassageFrame(QWidget *parent = 0);
    ~BiblePassageFrame();
    void init();
    VerseUrl toVerseUrl();
    void setVerseUrl(VerseUrl url);
    void setVerseUrlRanges(const QList<VerseUrlRange> &ranges);

private:
    Ui::BiblePassageFrame *ui;
    int m_count;
    int m_moduleID;
    QStringList m_bookNames;
};

#endif // BIBLEPASSAGEFRAME_H
