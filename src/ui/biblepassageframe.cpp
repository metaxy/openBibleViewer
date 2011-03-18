/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "biblepassageframe.h"
#include "ui_biblepassageframe.h"

BiblePassageFrame::BiblePassageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BiblePassageFrame)
{
    ui->setupUi(this);
    m_count = 0;
}

BiblePassageFrame::~BiblePassageFrame()
{
    delete ui;
}
void BiblePassageFrame::init()
{
    ModuleModel m(this);
    m.setSettings(m_settings);
    m.generate();
    m_model = m.itemModel();
    m_proxyModel = new RecursivProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->treeView->setModel(m_proxyModel);
}

void BiblePassageFrame::addBox_BCV(const int bookID, const int chapterID, const int verseID)
{
    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_" + QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_" + QString::number(m_count));
    QSpinBox *verse = new QSpinBox(this);
    verse->setObjectName("verse_" + QString::number(m_count));
    QToolButton *button = newButton(m_count);

    books->insertItems(0, m_bookNames);
    books->setCurrentIndex(bookID);
    chapter->setValue(chapterID + 1);
    verse->setValue(verseID + 1);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_" + QString::number(m_count));
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(verse);
    layout->addWidget(button);
    ui->verticalLayout->addLayout(layout);
    m_count++;

}
void BiblePassageFrame::addBox_BCVV(const int bookID, const int chapterID, const int startVerseID, const int endVerseID)
{
    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_" + QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_" + QString::number(m_count));
    QSpinBox *startVerse = new QSpinBox(this);
    startVerse->setObjectName("startVerse_" + QString::number(m_count));
    QSpinBox *endVerse = new QSpinBox(this);
    endVerse->setObjectName("endVerse_" + QString::number(m_count));
    QToolButton *button = newButton(m_count);

    books->insertItems(0, m_bookNames);
    books->setCurrentIndex(bookID);
    chapter->setValue(chapterID + 1);

    startVerse->setValue(startVerseID + 1);
    endVerse->setValue(endVerseID + 1);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_" + QString::number(m_count));
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(startVerse);
    layout->addWidget(endVerse);
    layout->addWidget(button);
    ui->verticalLayout->addLayout(layout);
    m_count++;
}
void BiblePassageFrame::addBox_BC(const int bookID, const int chapterID)
{
    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_" + QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_" + QString::number(m_count));

    books->insertItems(0, m_bookNames);
    books->setCurrentIndex(bookID);
    chapter->setValue(chapterID + 1);
    QToolButton *button = newButton(m_count);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_" + QString::number(m_count));
    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(button);
    ui->verticalLayout->addLayout(layout);
    m_count++;
}
void BiblePassageFrame::changeTypeTo_BC()
{
}
void BiblePassageFrame::changeTypeTo_BCV()
{

}
void BiblePassageFrame::changeTypeTo_BCVV()
{

}

QToolButton *BiblePassageFrame::newButton(const int id)
{
    QToolButton *button = new QToolButton(this);
    button->setObjectName("button_"+QString::number(id));
    button->setPopupMode(QToolButton::InstantPopup);

    QMenu *menu = new QMenu(this);
    QAction *actionDelete = new QAction(tr("Delete"), menu);
    actionDelete->setObjectName("delete_"+QString::number(id));
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteBox()));
    menu->addAction(actionDelete);

    QMenu *type = new QMenu(this);
    type->setTitle(tr("Type"));
    QAction *actionVerse = new QAction(tr("One Verse"), this);
    actionVerse->setObjectName("actionVerse_"+QString::number(id));
    connect(actionVerse, SIGNAL(triggered()), this, SLOT(changeTypeTo_BCV()));

    QAction *actionVerseRange = new QAction(tr("Verse Range"), this);
    actionVerseRange->setObjectName("actionVerseRange_"+QString::number(id));
    connect(actionVerseRange, SIGNAL(triggered()), this, SLOT(changeTypeTo_BCVV()));

    QAction *actionChapter = new QAction(tr("Chapter"), this);
    actionChapter->setObjectName("actionChapter_"+QString::number(id));
    connect(actionChapter, SIGNAL(triggered()), this, SLOT(changeTypeTo_BC()));

    type->addAction(actionVerse);
    type->addAction(actionVerseRange);
    type->addAction(actionChapter);

    menu->addMenu(type);

    button->setMenu(menu);
    return button;
}
void BiblePassageFrame::deleteBox()
{
    //todo:sender
}

VerseUrl BiblePassageFrame::toVerseUrl()
{
    DEBUG_FUNC_NAME
    VerseUrl url;
    QMap<int, int> books, chapter, verse, endVerse;
    foreach(QObject * o, this->children()) {
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
            chapter[id] = ((QSpinBox*)o)->value() - 1;
        } else if(o->objectName().startsWith("verse")) {
            verse[id] = ((QSpinBox*)o)->value() - 1;
        } else if(o->objectName().startsWith("startVerse")) {
            verse[id] = ((QSpinBox*)o)->value() - 1;
        } else if(o->objectName().startsWith("endVerse")) {
            endVerse[id] = ((QSpinBox*)o)->value() - 1;
        }

    }
    QMapIterator<int, int> it(books);
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
    myDebug() << url.toString();
    setVerseUrlRanges(url.ranges());
}
void BiblePassageFrame::setVerseUrlRanges(const QList<VerseUrlRange> &ranges)
{
    if(!ranges.isEmpty()) {
        m_moduleID = ranges.first().moduleID();
        Versification *v = m_settings->getModuleSettings(m_moduleID)->getV11n();
        if(v != NULL) {
            m_bookNames = v->bookNames().values();//todo:
        }
        QModelIndexList list = m_proxyModel->match(m_model->invisibleRootItem()->index(),
                               Qt::UserRole + 1,
                               QString::number(m_moduleID),
                               1 ,
                               Qt::MatchExactly);
        myDebug() << "list.size() = " << list.size();
        if(!list.isEmpty()) {
            myDebug() << list.first().data(Qt::UserRole + 1);
            ui->treeView->selectionModel()->clear();
            ui->treeView->selectionModel()->select(list.first(), QItemSelectionModel::Select);
        }
    }
    foreach(const VerseUrlRange & r, ranges) {
        m_moduleID = r.moduleID();
        if(r.startVerse() == VerseUrlRange::LoadFirstVerse && r.endVerse() == VerseUrlRange::LoadLastVerse) {
            addBox_BC(r.bookID(), r.chapterID());
        } else if(r.startVerseID() != r.endVerseID()) {
            addBox_BCVV(r.bookID(), r.chapterID(), r.startVerseID(), r.endVerseID());
        } else {
            addBox_BCV(r.bookID(), r.chapterID(), r.startVerseID());
        }
    }

}
