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
void BiblePassageFrame::addVerse(const int bookID, const int chapterID, const int verseID)
{
    QComboBox *books = new QComboBox(this);
    QSpinBox *chapter = new QSpinBox(this);
    QSpinBox *verse = new QSpinBox(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(verse);
    ui->verticalLayout->addLayout(layout);
    m_count++;

}
VerseUrl BiblePassageFrame::toVerseUrl()
{
    return VerseUrl();
}
void BiblePassageFrame::setVerseUrl(VerseUrl url)
{
    foreach(const VerseUrlRange &r,url.ranges()) {
        //addVerse(r.bookID(), r)
    }
}
