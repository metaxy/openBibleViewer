#include "poschoser.h"
#include <QtCore/QtDebug>

#include "ui_poschoser.h"

posChoser::posChoser(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::posChoser)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_save, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_ui->pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

posChoser::~posChoser()
{
    delete m_ui;
}
void posChoser::setData(QStringList bibles, QStringList books)
{
    m_ui->comboBox_books->insertItems(0, books);
    m_bibles = bibles;
    return;
}
void posChoser::setCurrent(int bible, QString path, int book, int chapter, int verse)
{
    //todo:load the bible info and from that the booknames
    m_bibleID = bible;
    m_bookID = book;
    m_chapterID = chapter;
    m_verseID = verse;
    m_path = path;
    m_ui->label_bible->setText(m_bibles.at(bible));
    m_ui->comboBox_books->setCurrentIndex(book);
    m_ui->spinBox_chapter->setValue(chapter);
    m_ui->spinBox_verse->setValue(verse);
    return;
}
void posChoser::save()
{
    QString out = "";
    out += m_path + ";";
    out += QString::number(m_ui->comboBox_books->currentIndex()) + ";";
    out += QString::number(m_ui->spinBox_chapter->value()) + ";";
    out += QString::number(m_ui->spinBox_verse->value()) + ";";
    close();
    emit updated(out);
    return;
}
void posChoser::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
