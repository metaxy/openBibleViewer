#ifndef POSCHOSER_H
#define POSCHOSER_H
#include "../core/chapter.h"
#include <QtGui/QDialog>

namespace Ui {
        class posChoser;
}

class posChoser : public QDialog {
        Q_OBJECT
public:
        explicit posChoser(QWidget *parent = 0);
        virtual ~posChoser();
        void setData(QStringList bibles,QStringList books);
        void setCurrent(int bible,QString path,int book,int chapter,int verse);

protected:
        virtual void changeEvent(QEvent *e);
public slots:
        void save();
signals:
        void updated(QString pos);
private:
        Ui::posChoser *m_ui;
        QStringList m_bibles;
        int m_bibleID,m_bookID,m_chapterID,m_verseID;
        QString m_path;
};

#endif // POSCHOSER_H
