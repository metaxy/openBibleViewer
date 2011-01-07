/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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
#include "versification_kjv.h"

Versification_KJV::Versification_KJV()
{
    {
        BookV11N book;
        book.name =  tr("Genesis");
        book.shortNames.append(tr("Gen, Ge, Gn").split(", "));
        book.maxChapter = 50;
        book.maxVerse  = (QList<int>() << 31 << 25 << 24 << 26 << 32 << 22 << 24 << 22 << 29 << 32 << 32 << 20 << 18 << 24 << 21 << 16 << 27 << 33 << 38 << 18 << 34 << 24 << 20 << 67 << 34 << 35 << 46 << 22 << 35 << 43 << 55 << 32 << 20 << 31 << 29 << 43 << 36 << 30 << 23 << 23 << 57 << 38 << 34 << 34 << 28 << 34 << 31 << 22 << 33 << 26);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }
    {
        BookV11N book;
        book.name =  tr("Exodus");
        book.shortNames.append(tr("Ex, Exo, Exd").split(", "));
        book.maxChapter = 40;
        book.maxVerse  = (QList<int>() << 22 << 25 << 22 << 31 << 23 << 30 << 25 << 32 << 35 << 29 << 10 << 51 << 22 << 31 << 27 << 36 << 16 << 27 << 25 << 26 << 36 << 31 << 33 << 18 << 40 << 37 << 21 << 43 << 46 << 38 << 18 << 35 << 23 << 35 << 35 << 38 << 29 << 31 << 43 << 38);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Leviticus");
        book.shortNames.append(tr("Lev, Le, Lv").split(", "));
        book.maxChapter = 27;
        book.maxVerse  = (QList<int>() << 17 << 16 << 17 << 35 << 19 << 30 << 38 << 36 << 24 << 20 << 47 << 8 << 59 << 57 << 33 << 34 << 16 << 30 << 37 << 27 << 24 << 33 << 44 << 23 << 55 << 46 << 34);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Numbers");
        book.shortNames.append(tr("Num, Nu, Nm, Nb").split(", "));
        book.maxChapter = 36;
        book.maxVerse  = (QList<int>() << 54 << 34 << 51 << 49 << 31 << 27 << 89 << 26 << 23 << 36 << 35 << 16 << 33 << 45 << 41 << 50 << 13 << 32 << 22 << 29 << 35 << 41 << 30 << 25 << 18 << 65 << 23 << 31 << 40 << 16 << 54 << 42 << 56 << 29 << 34 << 13);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Deuteronomy");
        book.shortNames.append(tr("Deut, Dt").split(", "));
        book.maxChapter = 34;
        book.maxVerse  = (QList<int>() << 46 << 37 << 29 << 49 << 33 << 25 << 26 << 20 << 29 << 22 << 32 << 32 << 18 << 29 << 23 << 22 << 20 << 22 << 21 << 20 << 23 << 30 << 25 << 22 << 19 << 19 << 26 << 68 << 29 << 20 << 30 << 52 << 29 << 12);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Joshua");
        book.shortNames.append(tr("Josh, Jos, Jsh").split(", "));
        book.maxChapter = 24;
        book.maxVerse  = (QList<int>() << 18 << 24 << 17 << 24 << 15 << 27 << 26 << 35 << 27 << 43 << 23 << 24 << 33 << 15 << 63 << 10 << 18 << 28 << 51 << 9 << 45 << 34 << 16 << 33);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Judges");
        book.shortNames.append(tr("Judg, Jdg, Jg, Jdgs").split(", "));
        book.maxChapter = 21;
        book.maxVerse  = (QList<int>() << 36 << 23 << 31 << 24 << 31 << 40 << 25 << 35 << 57 << 18 << 40 << 15 << 25 << 20 << 20 << 31 << 13 << 31 << 30 << 48 << 25);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Ruth");
        book.shortNames.append(tr("Rth, Ru").split(", "));
        book.maxChapter = 4;
        book.maxVerse  = (QList<int>() << 22 << 23 << 18 << 22);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Samuel");
        book.shortNames.append(tr("1 Sam, 1 Sa, 1Samuel, 1S, I Sa, 1 Sm, 1Sa, I Sam, 1Sam").split(", "));
        book.maxChapter = 31;
        book.maxVerse  = (QList<int>() << 28 << 36 << 21 << 22 << 12 << 21 << 17 << 22 << 27 << 27 << 15 << 25 << 23 << 52 << 35 << 23 << 58 << 30 << 24 << 42 << 15 << 23 << 29 << 22 << 44 << 25 << 12 << 25 << 11 << 31 << 13);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Samuel");
        book.shortNames.append(tr("2 Sam, 2 Sa, 2S, II Sa, 2 Sm, 2Sa, II Sam, 2Sam").split(", "));
        book.maxChapter = 24;
        book.maxVerse  = (QList<int>() << 27 << 32 << 39 << 12 << 25 << 23 << 29 << 18 << 13 << 19 << 27 << 31 << 39 << 33 << 37 << 23 << 29 << 33 << 43 << 26 << 22 << 51 << 39 << 25);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Kings");
        book.shortNames.append(tr("1 Kgs, 1 Ki, 1K, I Kgs, 1Kgs, I Ki, 1Ki, I Kings, 1Kings, 1st Kgs, 1st Kings, 1Kin").split(", "));
        book.maxChapter = 22;
        book.maxVerse  = (QList<int>() << 53 << 46 << 28 << 34 << 18 << 38 << 51 << 66 << 28 << 29 << 43 << 33 << 34 << 31 << 34 << 34 << 24 << 46 << 21 << 43 << 29 << 53);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Kings");
        book.shortNames.append(tr("2 Kgs, 2 Ki, 2K, II Kgs, 2Kgs, II Ki, 2Ki, II Kings, 2Kings, 2nd Kgs, 2nd Kings, 2Kin").split(", "));
        book.maxChapter = 25;
        book.maxVerse  = (QList<int>() << 18 << 25 << 27 << 44 << 27 << 33 << 20 << 29 << 37 << 36 << 21 << 21 << 25 << 29 << 38 << 20 << 41 << 37 << 37 << 21 << 26 << 20 << 37 << 20 << 30);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Chronicles");
        book.shortNames.append(tr("1 Chron, 1 Ch, I Ch, 1Ch, 1 Chr, I Chr, 1Chr, I Chron, 1Chron").split(", "));
        book.maxChapter = 29;
        book.maxVerse  = (QList<int>() << 54 << 55 << 24 << 43 << 26 << 81 << 40 << 40 << 44 << 14 << 47 << 40 << 14 << 17 << 29 << 43 << 27 << 17 << 19 << 8 << 30 << 19 << 32 << 31 << 31 << 32 << 34 << 21 << 30);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Chronicles");
        book.shortNames.append(tr("2 Chron, 2 Ch, II Ch, 2Ch, II Chr, 2Chr, II Chron, 2Chron").split(", "));
        book.maxChapter = 36;
        book.maxVerse  = (QList<int>() << 17 << 18 << 17 << 22 << 14 << 42 << 22 << 18 << 31 << 19 << 23 << 16 << 22 << 15 << 19 << 14 << 19 << 34 << 11 << 37 << 20 << 12 << 21 << 27 << 28 << 23 << 9 << 27 << 36 << 27 << 21 << 33 << 25 << 33 << 27 << 23);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Ezra");
        book.shortNames.append(tr("Ezra, Ezr").split(", "));
        book.maxChapter = 10;
        book.maxVerse  = (QList<int>() << 11 << 70 << 13 << 24 << 17 << 22 << 28 << 36 << 15 << 44);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Nehemiah");
        book.shortNames.append(tr("Neh, Ne").split(", "));
        book.maxChapter = 13;
        book.maxVerse  = (QList<int>() << 11 << 20 << 32 << 23 << 19 << 19 << 73 << 18 << 38 << 39 << 36 << 47 << 31);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Esther");
        book.shortNames.append(tr("Esth, Es").split(", "));
        book.maxChapter = 10;
        book.maxVerse  = (QList<int>() << 22 << 23 << 15 << 17 << 14 << 14 << 10 << 17 << 32 << 3);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Job");
        book.shortNames.append(tr("Job, Job, Jb").split(", "));
        book.maxChapter = 42;
        book.maxVerse  = (QList<int>() << 22 << 13 << 26 << 21 << 27 << 30 << 21 << 22 << 35 << 22 << 20 << 25 << 28 << 22 << 35 << 22 << 16 << 21 << 29 << 29 << 34 << 30 << 17 << 25 << 6 << 14 << 23 << 28 << 25 << 31 << 40 << 22 << 33 << 37 << 16 << 33 << 24 << 41 << 30 << 24 << 34 << 17);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Psalm");
        book.shortNames.append(tr("Pslm, Ps, Psalms, Psa, Psm, Pss").split(", "));
        book.maxChapter = 150;
        book.maxVerse  = (QList<int>() << 6 << 12 << 8 << 8 << 12 << 10 << 17 << 9 << 20 << 18 << 7 << 8 << 6 << 7 << 5 << 11 << 15 << 50 << 14 << 9 << 13 << 31 << 6 << 10 << 22 << 12 << 14 << 9 << 11 << 12 << 24 << 11 << 22 << 22 << 28 << 12 << 40 << 22 << 13 << 17 << 13 << 11 << 5 << 26 << 17 << 11 << 9 << 14 << 20 << 23 << 19 << 9 << 6 << 7 << 23 << 13 << 11 << 11 << 17 << 12 << 8 << 12 << 11 << 10 << 13 << 20 << 7 << 35 << 36 << 5 << 24 << 20 << 28 << 23 << 10 << 12 << 20 << 72 << 13 << 19 << 16 << 8 << 18 << 12 << 13 << 17 << 7 << 18 << 52 << 17 << 16 << 15 << 5 << 23 << 11 << 13 << 12 << 9 << 9 << 5 << 8 << 28 << 22 << 35 << 45 << 48 << 43 << 13 << 31 << 7 << 10 << 10 << 9 << 8 << 18 << 19 << 2 << 29 << 176 << 7 << 8 << 9 << 4 << 8 << 5 << 6 << 5 << 6 << 8 << 8 << 3 << 18 << 3 << 3 << 21 << 26 << 9 << 8 << 24 << 13 << 10 << 7 << 12 << 15 << 21 << 10 << 20 << 14 << 9 << 6);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Proverbs");
        book.shortNames.append(tr("Prov, Pr, Prv").split(", "));
        book.maxChapter = 31;
        book.maxVerse  = (QList<int>() << 33 << 22 << 35 << 27 << 23 << 35 << 27 << 36 << 18 << 32 << 31 << 28 << 25 << 35 << 33 << 33 << 28 << 24 << 29 << 30 << 31 << 29 << 35 << 34 << 28 << 28 << 27 << 28 << 27 << 33 << 31);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Ecclesiastes");
        book.shortNames.append(tr("Eccles, Ec, Qoh, Qoheleth").split(", "));
        book.maxChapter = 12;
        book.maxVerse  = (QList<int>() << 18 << 26 << 22 << 16 << 20 << 12 << 29 << 17 << 18 << 20 << 10 << 14);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Song of Solomon");
        book.shortNames.append(tr("Song, So, SOS").split(", "));
        book.maxChapter = 8;
        book.maxVerse  = (QList<int>() << 17 << 17 << 11 << 16 << 16 << 13 << 13 << 14);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Isaiah");
        book.shortNames.append(tr("Isa, Is").split(", "));
        book.maxChapter = 66;
        book.maxVerse  = (QList<int>() << 31 << 22 << 26 << 6 << 30 << 13 << 25 << 22 << 21 << 34 << 16 << 6 << 22 << 32 << 9 << 14 << 14 << 7 << 25 << 6 << 17 << 25 << 18 << 23 << 12 << 21 << 13 << 29 << 24 << 33 << 9 << 20 << 24 << 17 << 10 << 22 << 38 << 22 << 8 << 31 << 29 << 25 << 28 << 28 << 25 << 13 << 15 << 22 << 26 << 11 << 23 << 15 << 12 << 17 << 13 << 12 << 21 << 14 << 21 << 22 << 11 << 12 << 19 << 12 << 25 << 24);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Jeremiah");
        book.shortNames.append(tr("Jer, Je, Jr").split(", "));
        book.maxChapter = 52;
        book.maxVerse  = (QList<int>() << 19 << 37 << 25 << 31 << 31 << 30 << 34 << 22 << 26 << 25 << 23 << 17 << 27 << 22 << 21 << 21 << 27 << 23 << 15 << 18 << 14 << 30 << 40 << 10 << 38 << 24 << 22 << 17 << 32 << 24 << 40 << 44 << 26 << 22 << 19 << 32 << 21 << 28 << 18 << 16 << 18 << 22 << 13 << 30 << 5 << 28 << 7 << 47 << 39 << 46 << 64 << 34);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Lamentations");
        book.shortNames.append(tr("Lam, La").split(", "));
        book.maxChapter = 5;
        book.maxVerse  = (QList<int>() << 22 << 22 << 66 << 22 << 22);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Ezekiel");
        book.shortNames.append(tr("Ezek, Eze, Ezk").split(", "));
        book.maxChapter = 48;
        book.maxVerse  = (QList<int>() << 28 << 10 << 27 << 17 << 17 << 14 << 27 << 18 << 11 << 22 << 25 << 28 << 23 << 23 << 8 << 63 << 24 << 32 << 14 << 49 << 32 << 31 << 49 << 27 << 17 << 21 << 36 << 26 << 21 << 26 << 18 << 32 << 33 << 31 << 15 << 38 << 28 << 23 << 29 << 49 << 26 << 20 << 27 << 31 << 25 << 24 << 23 << 35);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Daniel");
        book.shortNames.append(tr("Dan, Da, Dn").split(", "));
        book.maxChapter = 12;
        book.maxVerse  = (QList<int>() << 21 << 49 << 30 << 37 << 31 << 28 << 28 << 27 << 27 << 21 << 45 << 13);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Hosea");
        book.shortNames.append(tr("Hos, Ho").split(", "));
        book.maxChapter = 14;
        book.maxVerse  = (QList<int>() << 11 << 23 << 5 << 19 << 15 << 11 << 16 << 14 << 17 << 15 << 12 << 14 << 16 << 9);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Joel");
        book.shortNames.append(tr("Joel, Joe, Jl").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 20 << 32 << 21);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Amos");
        book.shortNames.append(tr("Amos, Am").split(", "));
        book.maxChapter = 9;
        book.maxVerse  = (QList<int>() << 15 << 16 << 15 << 13 << 27 << 14 << 17 << 14 << 15);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Obadiah");
        book.shortNames.append(tr("Obad, Ob").split(", "));
        book.maxChapter = 1;
        book.maxVerse  = (QList<int>() << 21);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Jonah");
        book.shortNames.append(tr("Jnh, Jon").split(", "));
        book.maxChapter = 4;
        book.maxVerse  = (QList<int>() << 17 << 10 << 10 << 11);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Micah");
        book.shortNames.append(tr("Micah, Mic").split(", "));
        book.maxChapter = 7;
        book.maxVerse  = (QList<int>() << 16 << 13 << 12 << 13 << 15 << 16 << 20);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Nahum");
        book.shortNames.append(tr("Nah, Na").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 15 << 13 << 19);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Habakkuk");
        book.shortNames.append(tr("Hab, Hab").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 17 << 20 << 19);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Zephaniah");
        book.shortNames.append(tr("Zeph, Zep, Zp").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 18 << 15 << 20);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Haggai");
        book.shortNames.append(tr("Haggai, Hag, Hg").split(", "));
        book.maxChapter = 2;
        book.maxVerse  = (QList<int>() << 15 << 23);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Zechariah");
        book.shortNames.append(tr("Zech, Zec, Zc").split(", "));
        book.maxChapter = 14;
        book.maxVerse  = (QList<int>() << 21 << 13 << 10 << 14 << 11 << 15 << 14 << 23 << 17 << 12 << 17 << 14 << 9 << 21);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Malachi");
        book.shortNames.append(tr("Mal, Mal, Ml").split(", "));
        book.maxChapter = 4;
        book.maxVerse  = (QList<int>() << 14 << 17 << 18 << 6);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Matthew");
        book.shortNames.append(tr("Matt, Mt").split(", "));
        book.maxChapter = 28;
        book.maxVerse  = (QList<int>() << 25 << 23 << 17 << 25 << 48 << 34 << 29 << 34 << 38 << 42 << 30 << 50 << 58 << 36 << 39 << 28 << 27 << 35 << 30 << 34 << 46 << 46 << 39 << 51 << 46 << 75 << 66 << 20);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Mark");
        book.shortNames.append(tr("Mrk, Mk, Mr").split(", "));
        book.maxChapter = 16;
        book.maxVerse  = (QList<int>() << 45 << 28 << 35 << 41 << 43 << 56 << 37 << 38 << 50 << 52 << 33 << 44 << 37 << 72 << 47 << 20);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Luke");
        book.shortNames.append(tr("Luk, Lk").split(", "));
        book.maxChapter = 24;
        book.maxVerse  = (QList<int>() << 80 << 52 << 38 << 44 << 39 << 49 << 50 << 56 << 62 << 42 << 54 << 59 << 35 << 35 << 32 << 31 << 37 << 43 << 48 << 47 << 38 << 71 << 56 << 53);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("John");
        book.shortNames.append(tr("John, Jn, Jhn").split(", "));
        book.maxChapter = 21;
        book.maxVerse  = (QList<int>() << 51 << 25 << 36 << 54 << 47 << 71 << 53 << 59 << 41 << 42 << 57 << 50 << 38 << 31 << 27 << 33 << 26 << 40 << 42 << 31 << 25);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Acts");
        book.shortNames.append(tr("Acts, Ac").split(", "));
        book.maxChapter = 28;
        book.maxVerse  = (QList<int>() << 26 << 47 << 26 << 37 << 42 << 15 << 60 << 40 << 43 << 48 << 30 << 25 << 52 << 28 << 41 << 40 << 34 << 28 << 41 << 38 << 40 << 30 << 35 << 27 << 27 << 32 << 44 << 31);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }


    {
        BookV11N book;
        book.name =  tr("Romans");
        book.shortNames.append(tr("Rom, Ro, Rm").split(", "));
        book.maxChapter = 16;
        book.maxVerse  = (QList<int>() << 32 << 29 << 31 << 25 << 21 << 23 << 25 << 39 << 33 << 21 << 36 << 21 << 14 << 23 << 33 << 27);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Corinthians");
        book.shortNames.append(tr("1 Cor, 1 Co, I Co, 1Co, I Cor, 1Cor").split(", "));
        book.maxChapter = 16;
        book.maxVerse  = (QList<int>() << 31 << 16 << 23 << 21 << 13 << 20 << 40 << 13 << 27 << 33 << 34 << 31 << 13 << 40 << 58 << 24);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Corinthians");
        book.shortNames.append(tr("2 Cor, 2 Co, II Co, 2Co, II Cor, 2Cor").split(", "));
        book.maxChapter = 13;
        book.maxVerse  = (QList<int>() << 24 << 17 << 18 << 18 << 21 << 18 << 16 << 24 << 15 << 18 << 33 << 21 << 14);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Galatians");
        book.shortNames.append(tr("Gal, Ga").split(", "));
        book.maxChapter = 6;
        book.maxVerse  = (QList<int>() << 24 << 21 << 29 << 31 << 26 << 18);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Ephesians");
        book.shortNames.append(tr("Ephes, Eph").split(", "));
        book.maxChapter = 6;
        book.maxVerse  = (QList<int>() << 23 << 22 << 21 << 32 << 33 << 24);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Philippians");
        book.shortNames.append(tr("Phil, Php").split(", "));
        book.maxChapter = 4;
        book.maxVerse  = (QList<int>() << 30 << 30 << 21 << 23);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Colossians");
        book.shortNames.append(tr("Col, Col").split(", "));
        book.maxChapter = 4;
        book.maxVerse  = (QList<int>() << 29 << 23 << 25 << 18);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Thessalonians");
        book.shortNames.append(tr("1 Thess, 1 Th, I Th, 1Th, I Thes, 1Thes, I Thess, 1Thess").split(", "));
        book.maxChapter = 5;
        book.maxVerse  = (QList<int>() << 10 << 20 << 13 << 18 << 28);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Thessalonians");
        book.shortNames.append(tr("2 Thess, 2 Th, II Th, 2Th, II Thes, 2Thes, II Thess, 2Thess").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 12 << 17 << 18);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Timothy");
        book.shortNames.append(tr("1 Tim, 1 Ti, I Ti, 1Ti, I Tim, 1Tim").split(", "));
        book.maxChapter = 6;
        book.maxVerse  = (QList<int>() << 20 << 15 << 16 << 16 << 25 << 21);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Timothy");
        book.shortNames.append(tr("2 Tim, 2 Ti, II Ti, 2Ti, II Tim, 2Tim").split(", "));
        book.maxChapter = 4;
        book.maxVerse  = (QList<int>() << 18 << 26 << 17 << 22);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Titus");
        book.shortNames.append(tr("Titus, Tit").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 16 << 15 << 15);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Philemon");
        book.shortNames.append(tr("Philem, Phm").split(", "));
        book.maxChapter = 1;
        book.maxVerse  = (QList<int>() << 25);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Hebrews");
        book.shortNames.append(tr("Hebrews, Heb").split(", "));
        book.maxChapter = 13;
        book.maxVerse  = (QList<int>() << 14 << 18 << 19 << 16 << 14 << 20 << 28 << 13 << 28 << 39 << 40 << 29 << 25);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("James");
        book.shortNames.append(tr("James, Jas, Jm").split(", "));
        book.maxChapter = 5;
        book.maxVerse  = (QList<int>() << 27 << 26 << 18 << 17 << 20);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 Peter");
        book.shortNames.append(tr("1 Pet, 1 Pe, I Pe, 1Pe, I Pet, 1Pet, I Pt, 1 Pt, 1Pt").split(", "));
        book.maxChapter = 5;
        book.maxVerse  = (QList<int>() << 25 << 25 << 22 << 19 << 14);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 Peter");
        book.shortNames.append(tr("2 Pet, 2 Pe, II Pe, 2Pe, II Pet, 2Pet, II Pt, 2 Pt, 2Pt").split(", "));
        book.maxChapter = 3;
        book.maxVerse  = (QList<int>() << 21 << 22 << 18);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("1 John");
        book.shortNames.append(tr("1 John, 1 Jn, I Jn, 1Jn, I Jo, 1Jo, I Joh, 1Joh, I Jhn, 1 Jhn, 1Jhn, I John, 1John").split(", "));
        book.maxChapter = 5;
        book.maxVerse  = (QList<int>() << 10 << 29 << 24 << 21 << 21);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("2 John");
        book.shortNames.append(tr("2 John, 2 Jn, II Jn, 2Jn, II Jo, 2Jo, II Joh, 2Joh, II Jhn, 2 Jhn, 2Jhn, II John, 2John").split(", "));
        book.maxChapter = 1;
        book.maxVerse  = (QList<int>() << 13);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("3 John");
        book.shortNames.append(tr("3 John, 3 Jn, III Jn, 3Jn, III Jo, 3Jo, III Joh, 3Joh, III Jhn, 3 Jhn, 3Jhn, III John, 3John").split(", "));
        book.maxChapter = 1;
        book.maxVerse  = (QList<int>() << 14);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Jude");
        book.shortNames.append(tr("Jude, Jud").split(", "));
        book.maxChapter = 1;
        book.maxVerse  = (QList<int>() << 25);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

    {
        BookV11N book;
        book.name =  tr("Revelation");
        book.shortNames.append(tr("Rev, Re").split(", "));
        book.maxChapter = 22;
        book.maxVerse  = (QList<int>() << 20 << 29 << 22 << 11 << 14 << 17 << 17 << 13 << 21 << 11 << 19 << 17 << 18 << 20 << 8 << 21 << 18 << 24 << 21 << 15 << 27 << 21);
        book.bookID = 0;
        m_books.insert(book.bookID, book);
    }

}
Versification_KJV::~Versification_KJV()
{
}

bool Versification_KJV::filter(const int &bookID, VersificationFilterFlags filter) const
{
    if(filter.testFlag(Versification::ReturnAll) || (filter.testFlag(Versification::ReturnOT) && filter.testFlag(Versification::ReturnNT)))
        return true;
    //todo: do it
    return true;
}
