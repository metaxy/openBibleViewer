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
#pragma once

#include "src/core/basicclass.h"
#include "src/core/verseselection.h"
#include "src/core/verse/textranges.h"

/**
  ModuleAPI is a API to access to the bible and modulemanager
  */
class BibleWebChannel : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit BibleWebChannel(QObject *parent = 0);
    virtual ~BibleWebChannel();

    QString name() const;

    void setContent(const QString &html, const int &verseTableId);
    void getCurrentSelection(TextRanges textRanges);


signals:

    void newContent(const QString &html, const int &verseTableId);
    void getSelection();
    void verseSelectionReady(VerseSelection);

public slots:
    VerseSelection rawSelectionReady(int moduleId, int bookId, int startChapterId, int endChapterId, int startVerse, int endVerse, QString selectedText, QString startVerseText);


private: 
    TextRanges m_textRanges;

};

