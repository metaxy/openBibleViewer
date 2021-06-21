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
#ifndef VERSESELECTIONAPI_H
#define VERSESELECTIONAPI_H
#include <QObject>
#include "src/core/basicclass.h"
#include "src/core/verseselection.h"
#include "src/core/verse/textranges.h"


class VerseSelectionApi : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit VerseSelectionApi(QObject *parent = 0);
    QString name() const;
    QString connectorJS() const;

    void getCurrentSelection(TextRanges textRanges);

    int m_simpleStartVerse;
    int m_simpleEndVerse;
    QString m_simpleSelectedText;
    int m_simpleModuleID;
    int m_simpleBookID;
    int m_simpleChapterID;
    int m_simpleStartChapterID;
    int m_simpleEndChapterID;

    int m_adStartVerse = -1;
    QString m_adStartVerseText;
    QString m_adStartVerseContent;
    QString m_adSelectedText;

    TextRanges m_textRanges;
    
signals:
    void getSelection();
    void verseSelectionReady(VerseSelection);

public slots:
    VerseSelection rawSelectionReady();

};

#endif // VERSESELECTIONAPI_H
