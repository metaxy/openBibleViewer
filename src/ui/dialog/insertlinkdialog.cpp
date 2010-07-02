#include "insertlinkdialog.h"
#include "ui_insertlinkdialog.h"

InsertLinkDialog::InsertLinkDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::InsertLinkDialog)
{
    ui->setupUi(this);
    m_itemModel = new QStandardItemModel(ui->treeView);
    connect(ui->comboBox_bibles, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(save()));

}

InsertLinkDialog::~InsertLinkDialog()
{
    delete ui;
}
void InsertLinkDialog::init()
{

    m_itemModel->clear();
    QStringList id = m_notes->getIDList();

    QStandardItem *parentItem = m_itemModel->invisibleRootItem();
    for (int i = 0; i < id.size(); i++) {
        if (m_notes->getType(id.at(i)) == "text"/*really only text?*/) {
            QStandardItem *noteItem = new QStandardItem;
            noteItem->setText(m_notes->getTitle(id.at(i)));
            noteItem->setData(id.at(i));
            parentItem->appendRow(noteItem);
        }
    }

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_itemModel);
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Note Title"));
    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    ui->treeView->setSortingEnabled(true);
    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setSelectionModel(m_selectionModel);
}

void InsertLinkDialog::setCurrent(const int &bible, const QString &path, const int &book, const int &chapter, const int &verse)
{
    Q_UNUSED(bible);
    //DEBUG_FUNC_NAME
    ui->comboBox_bibles->insertItems(0, m_settings->getBibleNames());
    m_bookID = book;
    m_chapterID = chapter + 1;
    m_verseID = verse + 1;
    m_path = path;

    int newIndex = m_settings->getBiblePaths().lastIndexOf(path);
    ui->comboBox_bibles->setCurrentIndex(newIndex);//todo: if lastindexof == -1 show a warning
    ui->comboBox_books->setCurrentIndex(book);
    ui->spinBox_chapter->setValue(chapter);
    ui->spinBox_verse->setValue(verse);
}
void InsertLinkDialog::indexChanged(int index)
{
    //DEBUG_FUNC_NAME
    if (index >= 0) {
        ui->comboBox_books->clear();
        ui->comboBox_books->insertItems(0, m_settings->getBookNames().at(index));
        m_path = m_settings->getBiblePaths().at(index);


        ui->comboBox_books->setCurrentIndex(0);
        ui->spinBox_chapter->setValue(1);
        ui->spinBox_verse->setValue(1);
    } else {
        ui->comboBox_books->clear();
        ui->comboBox_books->setCurrentIndex(0);
        ui->spinBox_chapter->setValue(1);
        ui->spinBox_verse->setValue(1);
    }
}

void InsertLinkDialog::save()
{
    if (ui->toolBox->currentIndex() == 0) {
        QString link = m_path
                       + ";" + QString::number(ui->comboBox_books->currentIndex())
                       + ";" + QString::number(ui->spinBox_chapter->value() - 1)
                       + ";" + QString::number(ui->spinBox_verse->value() - 1)
                       + ";" + m_settings->getBookNames().at(ui->comboBox_bibles->currentIndex()).at(ui->comboBox_books->currentIndex());
        emit newLink("persistent://" + link);
    } else if (ui->toolBox->currentIndex() == 1) {
        QModelIndexList list = m_selectionModel->selectedRows(0);
        if (list.size() > 0) {
            QString id = list.at(0).data(Qt::UserRole + 1).toString();
            emit newLink("note://" + id);
        }
        ///get note id
    } else if (ui->toolBox->currentIndex() == 2) {
        emit newLink(ui->lineEdit->text());
    }
    close();
}
void InsertLinkDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
