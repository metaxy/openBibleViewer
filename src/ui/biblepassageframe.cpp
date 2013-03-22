/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "src/core/link/biblelink.h"
#include "src/ui/modulemodel.h"
BiblePassageFrame::BiblePassageFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BiblePassageFrame)
{
    ui->setupUi(this);
    m_count = 0;
    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->pushButton_replace, SIGNAL(clicked()), this, SLOT(replace()));
    connect(ui->treeView, SIGNAL(activated(QModelIndex)), this ,SLOT(reload(QModelIndex)));
}

BiblePassageFrame::~BiblePassageFrame()
{
    delete ui;
}
void BiblePassageFrame::init()
{
    ModuleModel m(this);
    m.setSettings(m_settings);
    m.addCategoryFilter(ModuleTools::BibleCategory);
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
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_" + QString::number(m_count));

    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_" + QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_" + QString::number(m_count));
    QSpinBox *verse = new QSpinBox(this);
    verse->setObjectName("verse_" + QString::number(m_count));
    QToolButton *button = newButton(m_count);

    QSharedPointer<Versification> v = m_settings->getModuleSettings(m_moduleID)->getV11n();
    if(v != NULL) {
        books->insertItems(0, v->bookNames().values());
        books->setCurrentIndex(v->bookIDs().indexOf(bookID));
    } else {
         myWarning() << "could not get v11n of " << m_moduleID;
    }

    chapter->setValue(chapterID + 1);
    verse->setValue(verseID + 1);

    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(verse);
    layout->addWidget(button);
    ui->verticalLayout->addLayout(layout);
    m_boxes.insert(m_count, layout);
    m_count++;

}
void BiblePassageFrame::addBox_BCVV(const int bookID, const int chapterID, const int startVerseID, const int endVerseID)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_" + QString::number(m_count));

    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_" + QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_" + QString::number(m_count));
    QSpinBox *startVerse = new QSpinBox(this);
    startVerse->setObjectName("startVerse_" + QString::number(m_count));
    QSpinBox *endVerse = new QSpinBox(this);
    endVerse->setObjectName("endVerse_" + QString::number(m_count));
    QToolButton *button = newButton(m_count);

    QSharedPointer<Versification> v = m_settings->getModuleSettings(m_moduleID)->getV11n();
    if(v != NULL) {
        books->insertItems(0, v->bookNames().values());
        books->setCurrentIndex(v->bookIDs().indexOf(bookID));
    } else {
        myWarning() << "could not get v11n of " << m_moduleID;
    }
    chapter->setValue(chapterID + 1);

    startVerse->setValue(startVerseID + 1);
    endVerse->setValue(endVerseID + 1);

    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(startVerse);
    layout->addWidget(endVerse);
    layout->addWidget(button);
    ui->verticalLayout->addLayout(layout);
    m_boxes.insert(m_count, layout);

    m_count++;
}
void BiblePassageFrame::addBox_BC(const int bookID, const int chapterID)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setObjectName("layout_" + QString::number(m_count));

    QComboBox *books = new QComboBox(this);
    books->setObjectName("books_" + QString::number(m_count));
    QSpinBox *chapter = new QSpinBox(this);
    chapter->setObjectName("chapter_" + QString::number(m_count));

    QSharedPointer<Versification> v = m_settings->getModuleSettings(m_moduleID)->getV11n();
    if(v != NULL) {
        books->insertItems(0, v->bookNames().values());
        books->setCurrentIndex(v->bookIDs().indexOf(bookID));
    } else {
        myWarning() << "could not get v11n of " << m_moduleID;
    }


    chapter->setValue(chapterID + 1);
    QToolButton *button = newButton(m_count);

    layout->addWidget(books);
    layout->addWidget(chapter);
    layout->addWidget(button);

    ui->verticalLayout->addLayout(layout);
    m_boxes.insert(m_count, layout);
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
    const QString sid = QString::number(id);
    QToolButton *button = new QToolButton(this);
    button->setObjectName("button_" + sid);
    button->setPopupMode(QToolButton::InstantPopup);
    button->setIcon(QIcon::fromTheme("configure", QIcon(":/icons/16x16/configure.png")));

    QMenu *menu = new QMenu(button);
    menu->setObjectName("menu_" + sid);
    QAction *actionDelete = new QAction(tr("Delete"), this);
    actionDelete->setObjectName("actionDelete_" + sid);
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteBox()));
    menu->addAction(actionDelete);

    QMenu *type = new QMenu(this);
    type->setObjectName("menuType_" + sid);
    type->setTitle(tr("Type"));
    QAction *actionVerse = new QAction(tr("One Verse"), this);
    actionVerse->setObjectName("actionVerse_" + sid);
    connect(actionVerse, SIGNAL(triggered()), this, SLOT(changeTypeTo_BCV()));

    QAction *actionVerseRange = new QAction(tr("Verse Range"), this);
    actionVerseRange->setObjectName("actionVerseRange_" + sid);
    connect(actionVerseRange, SIGNAL(triggered()), this, SLOT(changeTypeTo_BCVV()));

    QAction *actionChapter = new QAction(tr("Chapter"), this);
    actionChapter->setObjectName("actionChapter_" + sid);
    connect(actionChapter, SIGNAL(triggered()), this, SLOT(changeTypeTo_BC()));

    type->addAction(actionVerse);
    type->addAction(actionVerseRange);
    type->addAction(actionChapter);

    QMenu *newRange = new QMenu(this);
    newRange->setObjectName("menuNewRange" + sid);
    newRange->setTitle(tr("New"));

    QAction *actionNewVerse = new QAction(tr("One Verse"), this);
    actionNewVerse->setObjectName("actionNewVerse_" + sid);
    connect(actionNewVerse, SIGNAL(triggered()), this, SLOT(addBox_BCV()));

    QAction *actionNewVerseRange = new QAction(tr("Verse Range"), this);
    actionNewVerseRange->setObjectName("actionNewVerseRange_" + sid);
    connect(actionNewVerseRange, SIGNAL(triggered()), this, SLOT(addBox_BCVV()));

    QAction *actionNewChapter = new QAction(tr("Chapter"), this);
    actionNewChapter->setObjectName("actionNewChapter_" + sid);
    connect(actionNewChapter, SIGNAL(triggered()), this, SLOT(addBox_BC()));

    newRange->addAction(actionNewVerse);
    newRange->addAction(actionNewVerseRange);
    newRange->addAction(actionNewChapter);

    //menu->addMenu(type);
    menu->addMenu(newRange);

    button->setMenu(menu);
    return button;
}
void BiblePassageFrame::deleteBox()
{
    QString s = sender()->objectName();
    if(s.startsWith("actionDelete_")) {
        const int id = s.remove("actionDelete_").toInt();
        deleteBox(id);
    }
}
void BiblePassageFrame::deleteBox(const int boxID)
{
    QObject *button = NULL;
    foreach(QObject * o, this->children()) {
        if(o) {
            QString name = o->objectName();
            //myDebug() << "Name = " << name;
            QString bname = name;
            int pos = name.lastIndexOf("_");
            bool ok;
            int id = name.remove(0, pos + 1).toInt(&ok);
            if(ok && boxID == id && !bname.startsWith("action") && !bname.startsWith("menu")) {
                if(bname.startsWith("button")) {
                    button = o;
                } else {
                    //myDebug() << "deleted = " << bname;
                    //myDebug() << "nid = " << nid << "name = " << name;
                    delete o;
                }
            }
        }

    }
    if(button != NULL) {
        delete button;
    }
}

VerseUrl BiblePassageFrame::toVerseUrl()
{
    DEBUG_FUNC_NAME
    VerseUrl url;
    QMap<int, int> books, chapter, verse, endVerse;
    foreach(QObject * o, this->children()) {
        //myDebug() << o->objectName();

        int id = -1;
        if(o->objectName().contains("_")) {
            QStringList list = o->objectName().split("_");
            id = list.last().toInt();
        } else {
            continue;
        }
        if(o->objectName().startsWith("books")) {
            QSharedPointer<Versification> v = m_settings->getModuleSettings(m_moduleID)->getV11n();
            if(v != NULL) {
                books[id] = v->bookIDs().at(((QComboBox*)o)->currentIndex());
            }

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
    return url;
}
void BiblePassageFrame::setVerseUrl(const VerseUrl &url)
{
    DEBUG_FUNC_NAME
    myDebug() << url.toString();
    setVerseUrlRanges(url.ranges());
}
void BiblePassageFrame::setVerseUrlRanges(const QList<VerseUrlRange> &ranges)
{
    DEBUG_FUNC_NAME
    if(ranges.isEmpty()) {
        return;
    }
    m_moduleID = ranges.first().moduleID();

    myDebug() << "m_moduleID" << m_moduleID;
    QModelIndexList list = m_proxyModel->match(m_model->invisibleRootItem()->index(),
                           Qt::UserRole + 1,
                           QString::number(m_moduleID),
                           1,
                           Qt::MatchExactly);

    if(!list.isEmpty()) {
        ui->treeView->selectionModel()->clear();
        ui->treeView->selectionModel()->select(list.first(), QItemSelectionModel::Select);
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
void BiblePassageFrame::add()
{
    getModuleID();
    if(m_moduleID == -1) {
        return;
    }
    const QString text = ui->lineEdit->text();
    BibleLink link(m_moduleID, m_settings->getModuleSettings(m_moduleID)->getV11n());

    if(link.isBibleLink(text)) {
        setVerseUrl(link.getUrl(text));
    } else {

    }
}
void BiblePassageFrame::getModuleID()
{
    QModelIndexList list =ui->treeView->selectionModel()->selectedIndexes();
    if(!list.isEmpty()) {
        m_moduleID = list.first().data(Qt::UserRole + 1).toInt();
    }
}
void BiblePassageFrame::reload(const QModelIndex &index)
{
    m_moduleID = index.data(Qt::UserRole + 1).toInt();
    QSharedPointer<Versification> v = m_settings->getModuleSettings(m_moduleID)->getV11n();
    if(v.isNull()) {
        myWarning() << m_moduleID << index.data() << "no v11n";
        //clear the books
        foreach(QObject * o, this->children()) {
            if(o->objectName().startsWith("books")) {
                QComboBox *box = (QComboBox*)o;
                box->clear();
            }
        }
        return;
    }
    //todo: v11n conversion
    foreach(QObject * o, this->children()) {
        if(o->objectName().startsWith("books")) {
            QComboBox *box = (QComboBox*)o;
            int index = box->currentIndex();
            box->clear();
            box->insertItems(0, v->bookNames().values());
            box->setCurrentIndex(index);
        }
    }
}

void BiblePassageFrame::replace()
{
    clear();
    add();
}
void BiblePassageFrame::clear()
{
    for(int i = 0; i <= m_count;i++) {
        deleteBox(i);
    }
}
