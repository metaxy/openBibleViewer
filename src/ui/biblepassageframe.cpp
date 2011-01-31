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
    books->setObjectName("books_"+QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_"+QString::number(m_count));
    QSpinBox *verse = new QSpinBox(this);
    verse->setObjectName("verse_"+QString::number(m_count));
    books->insertItems(0, m_bookNames);
    books->setCurrentIndex(bookID);
    chapter->setValue(chapterID+1);
    verse->setValue(verseID+1);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_"+QString::number(m_count));
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(verse);
    ui->verticalLayout->addLayout(layout);
    m_count++;

}
void BiblePassageFrame::addMVerse(const int bookID, const int chapterID, const int startVerseID, const int endVerseID)
{
    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_"+QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_"+QString::number(m_count));
    QSpinBox *startVerse = new QSpinBox(this);
    startVerse->setObjectName("startVerse_"+QString::number(m_count));
    QSpinBox *endVerse = new QSpinBox(this);
    endVerse->setObjectName("endVerse_"+QString::number(m_count));

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
    DEBUG_FUNC_NAME
    VerseUrl url;
    QMap<int, int> books, chapter, verse, endVerse;
    foreach(QObject *o, this->children()) {
        myDebug() << o->objectName();

        int id = -1;
        if(o->objectName().contains("_")) {
            QStringList list = o->objectName().split("_");
            id = list.last().toInt();
        } else {
            continue;
        }
        if(o->objectName().startsWith("books")) {
            books[id] = ((QComboBox*)o)->currentIndex();

        } else if(o->objectName().startsWith("chapter")) {
            chapter[id] = ((QSpinBox*)o)->value()-1;
        } else if(o->objectName().startsWith("verse")) {
            verse[id] = ((QSpinBox*)o)->value()-1;
        } else if(o->objectName().startsWith("startVerse")) {
            verse[id] = ((QSpinBox*)o)->value()-1;
        } else if(o->objectName().startsWith("endVerse")) {
            endVerse[id] = ((QSpinBox*)o)->value()-1;
        }

    }
    QMapIterator<int,int> it(books);
    while(it.hasNext()) {
        it.next();
        if(it.key() > -1) {
            myDebug() << "key = " << it.key();
            VerseUrlRange r;
            r.setModule(m_moduleID);
            r.setBook(it.value());
            r.setChapter(chapter[it.key()]);
            r.setStartVerse(verse[it.key()]);
            if(endVerse.contains(it.key())) {
                r.setEndVerse(endVerse[it.key()]);
            }
            url.addRange(r);
        }
    }
    myDebug() << url.toString();
    return url;
}
void BiblePassageFrame::setVerseUrl(const VerseUrl &url)
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
