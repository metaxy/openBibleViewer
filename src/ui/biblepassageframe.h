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
#ifndef BIBLEPASSAGEFRAME_H
#define BIBLEPASSAGEFRAME_H

#include <QFrame>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include "src/core/basicclass.h"
#include "src/core/verse/verseurl.h"
#include "src/ui/recursivproxymodel.h"
namespace Ui
{
class BiblePassageFrame;
}

class BiblePassageFrame : public QFrame, public BasicClass
{
    Q_OBJECT
private slots:
    void addVerse(const int bookID = 0, const int chapterID = 0, const int verseID = 0);
    void addMVerse(const int bookID = 0, const int chapterID = 0, const int startVerseID = 0, const int endVerseID = 0);

public:
    explicit BiblePassageFrame(QWidget *parent = 0);
    ~BiblePassageFrame();
    void init();
    VerseUrl toVerseUrl();
    void setVerseUrl(const VerseUrl &url);
    void setVerseUrlRanges(const QList<VerseUrlRange> &ranges);

private:
    Ui::BiblePassageFrame *ui;
    int m_count;
    int m_moduleID;
    QStringList m_bookNames;
    RecursivProxyModel *m_proxyModel;
    QStandardItemModel *m_model;
};

#endif // BIBLEPASSAGEFRAME_H
