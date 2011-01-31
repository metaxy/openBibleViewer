#include "biblepassageframe.h"
#include "ui_biblepassageframe.h"

BiblePassageFrame::BiblePassageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BiblePassageFrame)
{
    ui->setupUi(this);
    m_count = 0;
    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(addVerse()));
}

BiblePassageFrame::~BiblePassageFrame()
{
    delete ui;
}
void BiblePassageFrame::init()
{
    ModuleModel m;
    m.setSettings(m_settings);
    m.generate();
    ui->treeView->setModel(m.itemModel());
}

void BiblePassageFrame::addVerse(const int bookID, const int chapterID, const int verseID)
{
    QComboBox *books = new QComboBox(this);
    QSpinBox *chapter = new QSpinBox(this);
    QSpinBox *verse = new QSpinBox(this);
    books->insertItems(0, m_bookNames);
    books->setCurrentIndex(bookID);
    chapter->setValue(chapterID+1);
    verse->setValue(verseID+1);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(verse);
    ui->verticalLayout->addLayout(layout);
    m_count++;

}
void BiblePassageFrame::addMVerse(const int bookID, const int chapterID, const int startVerseID, const int endVerseID)
{
    QComboBox *books = new QComboBox(this);
    QSpinBox *chapter = new QSpinBox(this);
    QSpinBox *startVerse = new QSpinBox(this);
    QSpinBox *endVerse = new QSpinBox(this);

    books->insertItems(0, m_bookNames);
    books->setCurrentIndex(bookID);
    chapter->setValue(chapterID+1);
    startVerse->setValue(startVerseID+1);
    endVerse->setValue(endVerseID+1);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_"+QString::number(m_count));
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(startVerse);
    layout->addWidget(endVerse);
    ui->verticalLayout->addLayout(layout);
    m_count++;

}
VerseUrl BiblePassageFrame::toVerseUrl()
{
    return VerseUrl();
}
void BiblePassageFrame::setVerseUrl(VerseUrl url)
{
    setVerseUrlRanges(url.ranges());
}
void BiblePassageFrame::setVerseUrlRanges(const QList<VerseUrlRange> &ranges)
{
    if(ranges.size() != 0) {
        Versification *v = m_settings->getModuleSettings(ranges.first().bibleID())->v11n;
        if(v != NULL) {
            m_bookNames = v->bookNames().values();//todo:
        }
    }
    foreach(const VerseUrlRange &r, ranges) {
        m_moduleID = r.bibleID();
        if(r.startVerseID() != r.endVerseID()) {
            addMVerse(r.bookID(), r.chapterID(),r.startVerseID(),r.endVerseID());
        } else {
            addVerse(r.bookID(), r.chapterID(),r.startVerseID());
        }
    }

}
