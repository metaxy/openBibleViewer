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
#include "verseselectionapi.h"
VerseSelectionApi::VerseSelectionApi(QObject *parent) :
    QObject(parent)
{
}

QString VerseSelectionApi::name() const
{
    return "VerseSelection";
}


QString VerseSelectionApi::connectorJS() const
{
    return "window.VerseSelection = channel.objects.VerseSelection;" \
        "VerseSelection.getSelection.connect(function() { simpleVerseGetSelection(); adVerseGetSelection(); VerseSelection.rawSelectionReady();});";
}

VerseSelection VerseSelectionApi::rawSelectionReady()
{
    DEBUG_FUNC_NAME
    VerseSelection s;
    s.moduleID = m_simpleModuleID;
    s.bookID  = m_simpleBookID;
    s.startChapterID = m_simpleStartChapterID;
    s.endChapterID = m_simpleEndChapterID;
    s.startVerse = m_simpleStartVerse;
    s.endVerse = m_simpleEndVerse;
    myDebug() << "start verse = " << s.startVerse << " end verse = " << s.endVerse;

    const QString startVerseText = m_textRanges.getVerse(s.bookID, s.startChapterID, s.startVerse).data();
    QString endVerseText;
    if(s.startVerse != s.endVerse || s.startChapterID != s.endChapterID)
        endVerseText = m_textRanges.getVerse(s.bookID, s.endChapterID, s.endVerse).data();
    else
        endVerseText = startVerseText;

    const QString selectedText = m_simpleSelectedText;
    myDebug() << "selected text = " << selectedText;
    myDebug() << "startVerseText = " << startVerseText;
    myDebug() << "endVerseText = " << endVerseText;
    {
        QString sText;
        for(int i = 0; i < selectedText.size() - 1; i++) {
            sText += selectedText.at(i);
            const int pos = startVerseText.indexOf(sText);
            if(pos != -1 && startVerseText.lastIndexOf(sText) == pos) {
                s.shortestStringInStartVerse = sText;
                break;
            }
        }
        if(s.shortestStringInStartVerse.isEmpty() && s.startVerse != s.endVerse) {
            //find the last long string if the selection is over more than one verse long
            //todo: it isn't alway unique
            QString lastLongest = selectedText;
            int lastPos = -2;
            for(int i = selectedText.size() - 1; i > 0; i--) {
                const int pos = startVerseText.lastIndexOf(lastLongest);
                if(pos != -1) {
                    lastPos = pos;
                    break;
                }
                lastLongest.remove(i, selectedText.size());
            }
            //and shorten it
            sText.clear();
            for(int i = 0; i < selectedText.size() - 1; i++) {
                sText += selectedText.at(i);
                const int pos = startVerseText.lastIndexOf(sText);
                if(pos != -1 && lastPos == pos) {
                    s.shortestStringInStartVerse = sText;
                    break;
                }
            }


        }
        sText.clear();
        for(int i = 0; i < selectedText.size() - 1; i++) {
            sText.prepend(selectedText.at(selectedText.size() - i - 1));
            const int pos = endVerseText.indexOf(sText);
            if(pos != -1 && endVerseText.lastIndexOf(sText) == pos) {
                s.shortestStringInEndVerse = sText;
                break;
            }
        }
        if(s.shortestStringInEndVerse.isEmpty() && s.startVerse != s.endVerse) {
            //find the first longest string if the selection is over more than one verse long
            QString firstLongest = selectedText;
            int firstPos = -2;
            for(int i = 0; i < selectedText.size(); i++) {
                const int pos = endVerseText.indexOf(firstLongest);
                if(pos != -1) {
                    firstPos = pos;
                    break;
                }
                firstLongest.remove(0, 1);
            }
            //and shorten it
            sText.clear();
            for(int i = 0; i < selectedText.size() - 1; i++) {
                sText.prepend(selectedText.at(selectedText.size() - i - 1));
                const int pos = endVerseText.indexOf(sText);
                if(pos != -1 && firstPos == pos) {
                    s.shortestStringInEndVerse = sText;
                    break;
                }
            }
        }
        s.type = VerseSelection::ShortestString;
        if(s.shortestStringInStartVerse.isEmpty() || s.shortestStringInEndVerse.isEmpty()) {
            s.setCanBeUsedForMarks(false);
        } else {
            s.setCanBeUsedForMarks(true);
        }
    }
    myDebug() << s.shortestStringInStartVerse << s.shortestStringInEndVerse;
    //do not this stuff with BibleQuote because some modules have weird html stuff.
    
    if(s.canBeUsedForMarks() == false) {
        //now the ultimative algo

        const QString startVerseText2 = m_adStartVerseText;
       
        const QString uniqueString = "!-_OPENBIBLEVIEWER_INSERT_-!";
        const int posOfInsert = startVerseText2.lastIndexOf(uniqueString);

        QString back = selectedText;
        QString longestString;
        for(int i = selectedText.size() - 1; i > 0; --i) {
            const int pos = startVerseText2.indexOf(back);
            if(pos != -1) {
                longestString = back;
                break;
            }
            back.remove(i, selectedText.size());
        }

        int count = 0;
        int currentPos = 0;
        while(true) {
            currentPos = startVerseText2.indexOf(longestString, currentPos + 1);
            if(currentPos > posOfInsert || currentPos == -1) {
                break;
            }
            count++;
        }
        s.type = VerseSelection::RepeatOfLongestString;
        s.repeat = count;
        s.longestString = longestString;
        if(!s.longestString.isEmpty())
            s.setCanBeUsedForMarks(true);
        myDebug() << "longest = " << longestString << " count = " << count;
        //TODO: f->runJavaScript("removeSelectionStuff()");
    }
    emit verseSelectionReady(s);
    return s;
}

void VerseSelectionApi::getCurrentSelection(TextRanges textRanges)
{
    DEBUG_FUNC_NAME
    m_simpleStartVerse = -1;
    m_simpleEndVerse = -1;
    m_simpleSelectedText = "";
    m_simpleModuleID = -1;
    m_simpleBookID = -1;
    m_simpleChapterID = -1;
    m_simpleStartChapterID = -1;
    m_simpleEndChapterID = -1;
    m_textRanges = textRanges;
    m_adStartVerse = -1;
    m_adStartVerseText = "";
    m_adStartVerseContent = "";
    m_adSelectedText = "not exec";

    emit getSelection();
}



