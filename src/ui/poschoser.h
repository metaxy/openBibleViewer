/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009 Paul Walger
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
#ifndef POSCHOSER_H
#define POSCHOSER_H
#include "../core/chapter.h"
#include <QtGui/QDialog>
#include "../core/settings.h"
namespace Ui
{
class posChoser;
}

class posChoser : public QDialog
{
    Q_OBJECT
public:
    explicit posChoser(QWidget *parent = 0);
    virtual ~posChoser();
    void setSettings(Settings *set);
    void setCurrent(int bible, QString path, int book, int chapter, int verse);

protected:
    virtual void changeEvent(QEvent *e);
public slots:
    void save();

    void indexChanged(int index);
signals:
    void updated(QString pos);
private:
    Ui::posChoser *m_ui;
    Settings *m_settings;
    QStringList m_bibles;
    int m_bibleID, m_bookID, m_chapterID, m_verseID;
    QString m_path;
};

#endif // POSCHOSER_H
