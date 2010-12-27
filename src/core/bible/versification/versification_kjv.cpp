#include "versification_kjv.h"

Versification_KJV::Versification_KJV()
{
    m_bookNames << tr("Genesis");
    m_bookShortNames.append(tr("Gen, Ge, Gn").split(", "));
    m_maxChapter << 50;


    m_bookNames << tr("Exodus");
    m_bookShortNames.append(tr("Ex, Exo, Exd").split(", "));
    m_maxChapter << 40;

    m_bookNames << tr("Leviticus");
    m_bookShortNames.append(tr("Lev, Le, Lv").split(", "));
    m_maxChapter << 27;

    m_bookNames << tr("Numbers");
    m_bookShortNames.append(tr("Num, Nu, Nm, Nb").split(", "));
    m_maxChapter << 36;

    m_bookNames << tr("Deuteronomy");
    m_bookShortNames.append(tr("Deut, Dt").split(", "));
    m_maxChapter << 34;

    m_bookNames << tr("Joshua");
    m_bookShortNames.append(tr("Josh, Jos, Jsh").split(", "));
    m_maxChapter << 24;

    m_bookNames << tr("Judges");
    m_bookShortNames.append(tr("Judg, Jdg, Jg, Jdgs").split(", "));
    m_maxChapter << 21;

    m_bookNames << tr("Ruth");
    m_bookShortNames.append(tr("Rth, Ru").split(", "));
    m_maxChapter << 4;

    m_bookNames << tr("1 Samuel");
    m_bookShortNames.append(tr("1 Sam, 1 Sa, 1Samuel, 1S, I Sa, 1 Sm, 1Sa, I Sam, 1Sam").split(", "));
    m_maxChapter << 31;

    m_bookNames << tr("2 Samuel");
    m_bookShortNames.append(tr("2 Sam, 2 Sa, 2S, II Sa, 2 Sm, 2Sa, II Sam, 2Sam").split(", "));
    m_maxChapter << 24;

    m_bookNames << tr("1 Kings");
    m_bookShortNames.append(tr("1 Kgs, 1 Ki, 1K, I Kgs, 1Kgs, I Ki, 1Ki, I Kings, 1Kings, 1st Kgs, 1st Kings, 1Kin").split(", "));
    m_maxChapter << 22;

    m_bookNames << tr("2 Kings");
    m_bookShortNames.append(tr("2 Kgs, 2 Ki, 2K, II Kgs, 2Kgs, II Ki, 2Ki, II Kings, 2Kings, 2nd Kgs, 2nd Kings, 2Kin").split(", "));
    m_maxChapter << 25;

    m_bookNames << tr("1 Chronicles");
    m_bookShortNames.append(tr("1 Chron, 1 Ch, I Ch, 1Ch, 1 Chr, I Chr, 1Chr, I Chron, 1Chron").split(", "));
    m_maxChapter << 29;

    m_bookNames << tr("2 Chronicles");
    m_bookShortNames.append(tr("2 Chron, 2 Ch, II Ch, 2Ch, II Chr, 2Chr, II Chron, 2Chron").split(", "));
    m_maxChapter << 36;

    m_bookNames << tr("Ezra");
    m_bookShortNames.append(tr("Ezra, Ezr").split(", "));
    m_maxChapter << 10;

    m_bookNames << tr("Nehemiah");
    m_bookShortNames.append(tr("Neh, Ne").split(", "));
    m_maxChapter << 13;

    m_bookNames << tr("Esther");
    m_bookShortNames.append(tr("Esth, Es").split(", "));
    m_maxChapter << 10;

    m_bookNames << tr("Job");
    m_bookShortNames.append(tr("Job, Job, Jb").split(", "));
    m_maxChapter << 42;

    m_bookNames << tr("Psalm");
    m_bookShortNames.append(tr("Pslm, Ps, Psalms, Psa, Psm, Pss").split(", "));
    m_maxChapter << 150;

    m_bookNames << tr("Proverbs");
    m_bookShortNames.append(tr("Prov, Pr, Prv").split(", "));
    m_maxChapter << 31;

    m_bookNames << tr("Ecclesiastes");
    m_bookShortNames.append(tr("Eccles, Ec, Qoh, Qoheleth").split(", "));
    m_maxChapter << 12;

    m_bookNames << tr("Song of Solomon");
    m_bookShortNames.append(tr("Song, So, SOS").split(", "));
    m_maxChapter << 8;

    m_bookNames << tr("Isaiah");
    m_bookShortNames.append(tr("Isa, Is").split(", "));
    m_maxChapter << 66;

    m_bookNames << tr("Jeremiah");
    m_bookShortNames.append(tr("Jer, Je, Jr").split(", "));
    m_maxChapter << 52;

    m_bookNames << tr("Lamentations");
    m_bookShortNames.append(tr("Lam, La").split(", "));
    m_maxChapter << 5;

    m_bookNames << tr("Ezekiel");
    m_bookShortNames.append(tr("Ezek, Eze, Ezk").split(", "));
    m_maxChapter << 48;

    m_bookNames << tr("Daniel");
    m_bookShortNames.append(tr("Dan, Da, Dn").split(", "));
    m_maxChapter << 12;

    m_bookNames << tr("Hosea");
    m_bookShortNames.append(tr("Hos, Ho").split(", "));
    m_maxChapter << 14;

    m_bookNames << tr("Joel");
    m_bookShortNames.append(tr("Joel, Joe, Jl").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("Amos");
    m_bookShortNames.append(tr("Amos, Am").split(", "));
    m_maxChapter << 9;

    m_bookNames << tr("Obadiah");
    m_bookShortNames.append(tr("Obad, Ob").split(", "));
    m_maxChapter << 1;

    m_bookNames << tr("Jonah");
    m_bookShortNames.append(tr("Jnh, Jon").split(", "));
    m_maxChapter << 4;

    m_bookNames << tr("Micah");
    m_bookShortNames.append(tr("Micah, Mic").split(", "));
    m_maxChapter << 7;

    m_bookNames << tr("Nahum");
    m_bookShortNames.append(tr("Nah, Na").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("Habakkuk");
    m_bookShortNames.append(tr("Hab, Hab").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("Zephaniah");
    m_bookShortNames.append(tr("Zeph, Zep, Zp").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("Haggai");
    m_bookShortNames.append(tr("Haggai, Hag, Hg").split(", "));
    m_maxChapter << 2;

    m_bookNames << tr("Zechariah");
    m_bookShortNames.append(tr("Zech, Zec, Zc").split(", "));
    m_maxChapter << 14;

    m_bookNames << tr("Malachi");
    m_bookShortNames.append(tr("Mal, Mal, Ml").split(", "));
    m_maxChapter << 4;

    m_bookNames << tr("Matthew");
    m_bookShortNames.append(tr("Matt, Mt").split(", "));
    m_maxChapter << 28;


    m_bookNames << tr("Mark");
    m_bookShortNames.append(tr("Mrk, Mk, Mr").split(", "));
    m_maxChapter << 16;

    m_bookNames << tr("Luke");
    m_bookShortNames.append(tr("Luk, Lk").split(", "));
    m_maxChapter << 24;

    m_bookNames << tr("John");
    m_bookShortNames.append(tr("John, Jn, Jhn").split(", "));
    m_maxChapter << 21;

    m_bookNames << tr("Acts");
    m_bookShortNames.append(tr("Acts, Ac").split(", "));
    m_maxChapter << 28;

    m_bookNames << tr("Romans");
    m_bookShortNames.append(tr("Rom, Ro, Rm").split(", "));
    m_maxChapter << 16;

    m_bookNames << tr("1 Corinthians");
    m_bookShortNames.append(tr("1 Cor, 1 Co, I Co, 1Co, I Cor, 1Cor").split(", "));
    m_maxChapter << 16;

    m_bookNames << tr("2 Corinthians");
    m_bookShortNames.append(tr("2 Cor, 2 Co, II Co, 2Co, II Cor, 2Cor").split(", "));
    m_maxChapter << 13;

    m_bookNames << tr("Galatians");
    m_bookShortNames.append(tr("Gal, Ga").split(", "));
    m_maxChapter << 6;

    m_bookNames << tr("Ephesians");
    m_bookShortNames.append(tr("Ephes, Eph").split(", "));
    m_maxChapter << 6;

    m_bookNames << tr("Philippians");
    m_bookShortNames.append(tr("Phil, Php").split(", "));
    m_maxChapter << 4;

    m_bookNames << tr("Colossians");
    m_bookShortNames.append(tr("Col, Col").split(", "));
    m_maxChapter << 4;

    m_bookNames << tr("1 Thessalonians");
    m_bookShortNames.append(tr("1 Thess, 1 Th, I Th, 1Th, I Thes, 1Thes, I Thess, 1Thess").split(", "));
    m_maxChapter << 5;

    m_bookNames << tr("2 Thessalonians");
    m_bookShortNames.append(tr("2 Thess, 2 Th, II Th, 2Th, II Thes, 2Thes, II Thess, 2Thess").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("1 Timothy");
    m_bookShortNames.append(tr("1 Tim, 1 Ti, I Ti, 1Ti, I Tim, 1Tim").split(", "));
    m_maxChapter << 6;

    m_bookNames << tr("2 Timothy");
    m_bookShortNames.append(tr("2 Tim, 2 Ti, II Ti, 2Ti, II Tim, 2Tim").split(", "));
    m_maxChapter << 4;

    m_bookNames << tr("Titus");
    m_bookShortNames.append(tr("Titus, Tit").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("Philemon");
    m_bookShortNames.append(tr("Philem, Phm").split(", "));
    m_maxChapter << 1;

    m_bookNames << tr("Hebrews");
    m_bookShortNames.append(tr("Hebrews, Heb").split(", "));
    m_maxChapter << 13;

    m_bookNames << tr("James");
    m_bookShortNames.append(tr("James, Jas, Jm").split(", "));
    m_maxChapter << 5;

    m_bookNames << tr("1 Peter");
    m_bookShortNames.append(tr("1 Pet, 1 Pe, I Pe, 1Pe, I Pet, 1Pet, I Pt, 1 Pt, 1Pt").split(", "));
    m_maxChapter << 5;

    m_bookNames << tr("2 Peter");
    m_bookShortNames.append(tr("2 Pet, 2 Pe, II Pe, 2Pe, II Pet, 2Pet, II Pt, 2 Pt, 2Pt").split(", "));
    m_maxChapter << 3;

    m_bookNames << tr("1 John");
    m_bookShortNames.append(tr("1 John, 1 Jn, I Jn, 1Jn, I Jo, 1Jo, I Joh, 1Joh, I Jhn, 1 Jhn, 1Jhn, I John, 1John").split(", "));
    m_maxChapter << 5;

    m_bookNames << tr("2 John");
    m_bookShortNames.append(tr("2 John, 2 Jn, II Jn, 2Jn, II Jo, 2Jo, II Joh, 2Joh, II Jhn, 2 Jhn, 2Jhn, II John, 2John").split(", "));
    m_maxChapter << 1;

    m_bookNames << tr("3 John");
    m_bookShortNames.append(tr("3 John, 3 Jn, III Jn, 3Jn, III Jo, 3Jo, III Joh, 3Joh, III Jhn, 3 Jhn, 3Jhn, III John, 3John").split(", "));
    m_maxChapter << 1;

    m_bookNames << tr("Jude");
    m_bookShortNames.append(tr("Jude, Jud").split(", "));
    m_maxChapter << 1;

    m_bookNames << tr("Revelation");
    m_bookShortNames.append(tr("Rev, Re").split(", "));
    m_maxChapter << 22;



    m_maxVerse << (QList<int>() << 31 << 25 << 24 << 26 << 32 << 22 << 24 << 22 << 29 << 32 << 32 << 20 << 18 << 24 << 21 << 16 << 27 << 33 << 38 << 18 << 34 << 24 << 20 << 67 << 34 << 35 << 46 << 22 << 35 << 43 << 55 << 32 << 20 << 31 << 29 << 43 << 36 << 30 << 23 << 23 << 57 << 38 << 34 << 34 << 28 << 34 << 31 << 22 << 33 << 26);
    m_maxVerse << (QList<int>() << 22 << 25 << 22 << 31 << 23 << 30 << 25 << 32 << 35 << 29 << 10 << 51 << 22 << 31 << 27 << 36 << 16 << 27 << 25 << 26 << 36 << 31 << 33 << 18 << 40 << 37 << 21 << 43 << 46 << 38 << 18 << 35 << 23 << 35 << 35 << 38 << 29 << 31 << 43 << 38);
    m_maxVerse << (QList<int>() << 17 << 16 << 17 << 35 << 19 << 30 << 38 << 36 << 24 << 20 << 47 << 8 << 59 << 57 << 33 << 34 << 16 << 30 << 37 << 27 << 24 << 33 << 44 << 23 << 55 << 46 << 34);
    m_maxVerse << (QList<int>() << 54 << 34 << 51 << 49 << 31 << 27 << 89 << 26 << 23 << 36 << 35 << 16 << 33 << 45 << 41 << 50 << 13 << 32 << 22 << 29 << 35 << 41 << 30 << 25 << 18 << 65 << 23 << 31 << 40 << 16 << 54 << 42 << 56 << 29 << 34 << 13);
    m_maxVerse << (QList<int>() << 46 << 37 << 29 << 49 << 33 << 25 << 26 << 20 << 29 << 22 << 32 << 32 << 18 << 29 << 23 << 22 << 20 << 22 << 21 << 20 << 23 << 30 << 25 << 22 << 19 << 19 << 26 << 68 << 29 << 20 << 30 << 52 << 29 << 12);
    m_maxVerse << (QList<int>() << 18 << 24 << 17 << 24 << 15 << 27 << 26 << 35 << 27 << 43 << 23 << 24 << 33 << 15 << 63 << 10 << 18 << 28 << 51 << 9 << 45 << 34 << 16 << 33);
    m_maxVerse << (QList<int>() << 36 << 23 << 31 << 24 << 31 << 40 << 25 << 35 << 57 << 18 << 40 << 15 << 25 << 20 << 20 << 31 << 13 << 31 << 30 << 48 << 25);
    m_maxVerse << (QList<int>() << 22 << 23 << 18 << 22);
    m_maxVerse << (QList<int>() << 28 << 36 << 21 << 22 << 12 << 21 << 17 << 22 << 27 << 27 << 15 << 25 << 23 << 52 << 35 << 23 << 58 << 30 << 24 << 42 << 15 << 23 << 29 << 22 << 44 << 25 << 12 << 25 << 11 << 31 << 13);
    m_maxVerse << (QList<int>() << 27 << 32 << 39 << 12 << 25 << 23 << 29 << 18 << 13 << 19 << 27 << 31 << 39 << 33 << 37 << 23 << 29 << 33 << 43 << 26 << 22 << 51 << 39 << 25);
    m_maxVerse << (QList<int>() << 53 << 46 << 28 << 34 << 18 << 38 << 51 << 66 << 28 << 29 << 43 << 33 << 34 << 31 << 34 << 34 << 24 << 46 << 21 << 43 << 29 << 53);
    m_maxVerse << (QList<int>() << 18 << 25 << 27 << 44 << 27 << 33 << 20 << 29 << 37 << 36 << 21 << 21 << 25 << 29 << 38 << 20 << 41 << 37 << 37 << 21 << 26 << 20 << 37 << 20 << 30);
    m_maxVerse << (QList<int>() << 54 << 55 << 24 << 43 << 26 << 81 << 40 << 40 << 44 << 14 << 47 << 40 << 14 << 17 << 29 << 43 << 27 << 17 << 19 << 8 << 30 << 19 << 32 << 31 << 31 << 32 << 34 << 21 << 30);
    m_maxVerse << (QList<int>() << 17 << 18 << 17 << 22 << 14 << 42 << 22 << 18 << 31 << 19 << 23 << 16 << 22 << 15 << 19 << 14 << 19 << 34 << 11 << 37 << 20 << 12 << 21 << 27 << 28 << 23 << 9 << 27 << 36 << 27 << 21 << 33 << 25 << 33 << 27 << 23);
    m_maxVerse << (QList<int>() << 11 << 70 << 13 << 24 << 17 << 22 << 28 << 36 << 15 << 44);
    m_maxVerse << (QList<int>() << 11 << 20 << 32 << 23 << 19 << 19 << 73 << 18 << 38 << 39 << 36 << 47 << 31);
    m_maxVerse << (QList<int>() << 22 << 23 << 15 << 17 << 14 << 14 << 10 << 17 << 32 << 3);
    m_maxVerse << (QList<int>() << 22 << 13 << 26 << 21 << 27 << 30 << 21 << 22 << 35 << 22 << 20 << 25 << 28 << 22 << 35 << 22 << 16 << 21 << 29 << 29 << 34 << 30 << 17 << 25 << 6 << 14 << 23 << 28 << 25 << 31 << 40 << 22 << 33 << 37 << 16 << 33 << 24 << 41 << 30 << 24 << 34 << 17);
    m_maxVerse << (QList<int>() << 6 << 12 << 8 << 8 << 12 << 10 << 17 << 9 << 20 << 18 << 7 << 8 << 6 << 7 << 5 << 11 << 15 << 50 << 14 << 9 << 13 << 31 << 6 << 10 << 22 << 12 << 14 << 9 << 11 << 12 << 24 << 11 << 22 << 22 << 28 << 12 << 40 << 22 << 13 << 17 << 13 << 11 << 5 << 26 << 17 << 11 << 9 << 14 << 20 << 23 << 19 << 9 << 6 << 7 << 23 << 13 << 11 << 11 << 17 << 12 << 8 << 12 << 11 << 10 << 13 << 20 << 7 << 35 << 36 << 5 << 24 << 20 << 28 << 23 << 10 << 12 << 20 << 72 << 13 << 19 << 16 << 8 << 18 << 12 << 13 << 17 << 7 << 18 << 52 << 17 << 16 << 15 << 5 << 23 << 11 << 13 << 12 << 9 << 9 << 5 << 8 << 28 << 22 << 35 << 45 << 48 << 43 << 13 << 31 << 7 << 10 << 10 << 9 << 8 << 18 << 19 << 2 << 29 << 176 << 7 << 8 << 9 << 4 << 8 << 5 << 6 << 5 << 6 << 8 << 8 << 3 << 18 << 3 << 3 << 21 << 26 << 9 << 8 << 24 << 13 << 10 << 7 << 12 << 15 << 21 << 10 << 20 << 14 << 9 << 6);
    m_maxVerse << (QList<int>() << 33 << 22 << 35 << 27 << 23 << 35 << 27 << 36 << 18 << 32 << 31 << 28 << 25 << 35 << 33 << 33 << 28 << 24 << 29 << 30 << 31 << 29 << 35 << 34 << 28 << 28 << 27 << 28 << 27 << 33 << 31);
    m_maxVerse << (QList<int>() << 18 << 26 << 22 << 16 << 20 << 12 << 29 << 17 << 18 << 20 << 10 << 14);
    m_maxVerse << (QList<int>() << 17 << 17 << 11 << 16 << 16 << 13 << 13 << 14);
    m_maxVerse << (QList<int>() << 31 << 22 << 26 << 6 << 30 << 13 << 25 << 22 << 21 << 34 << 16 << 6 << 22 << 32 << 9 << 14 << 14 << 7 << 25 << 6 << 17 << 25 << 18 << 23 << 12 << 21 << 13 << 29 << 24 << 33 << 9 << 20 << 24 << 17 << 10 << 22 << 38 << 22 << 8 << 31 << 29 << 25 << 28 << 28 << 25 << 13 << 15 << 22 << 26 << 11 << 23 << 15 << 12 << 17 << 13 << 12 << 21 << 14 << 21 << 22 << 11 << 12 << 19 << 12 << 25 << 24);
    m_maxVerse << (QList<int>() << 19 << 37 << 25 << 31 << 31 << 30 << 34 << 22 << 26 << 25 << 23 << 17 << 27 << 22 << 21 << 21 << 27 << 23 << 15 << 18 << 14 << 30 << 40 << 10 << 38 << 24 << 22 << 17 << 32 << 24 << 40 << 44 << 26 << 22 << 19 << 32 << 21 << 28 << 18 << 16 << 18 << 22 << 13 << 30 << 5 << 28 << 7 << 47 << 39 << 46 << 64 << 34);
    m_maxVerse << (QList<int>() << 22 << 22 << 66 << 22 << 22);
    m_maxVerse << (QList<int>() << 28 << 10 << 27 << 17 << 17 << 14 << 27 << 18 << 11 << 22 << 25 << 28 << 23 << 23 << 8 << 63 << 24 << 32 << 14 << 49 << 32 << 31 << 49 << 27 << 17 << 21 << 36 << 26 << 21 << 26 << 18 << 32 << 33 << 31 << 15 << 38 << 28 << 23 << 29 << 49 << 26 << 20 << 27 << 31 << 25 << 24 << 23 << 35);
    m_maxVerse << (QList<int>() << 21 << 49 << 30 << 37 << 31 << 28 << 28 << 27 << 27 << 21 << 45 << 13);
    m_maxVerse << (QList<int>() << 11 << 23 << 5 << 19 << 15 << 11 << 16 << 14 << 17 << 15 << 12 << 14 << 16 << 9);
    m_maxVerse << (QList<int>() << 20 << 32 << 21);
    m_maxVerse << (QList<int>() << 15 << 16 << 15 << 13 << 27 << 14 << 17 << 14 << 15);
    m_maxVerse << (QList<int>() << 21);
    m_maxVerse << (QList<int>() << 17 << 10 << 10 << 11);
    m_maxVerse << (QList<int>() << 16 << 13 << 12 << 13 << 15 << 16 << 20);
    m_maxVerse << (QList<int>() << 15 << 13 << 19);
    m_maxVerse << (QList<int>() << 17 << 20 << 19);
    m_maxVerse << (QList<int>() << 18 << 15 << 20);
    m_maxVerse << (QList<int>() << 15 << 23);
    m_maxVerse << (QList<int>() << 21 << 13 << 10 << 14 << 11 << 15 << 14 << 23 << 17 << 12 << 17 << 14 << 9 << 21);
    m_maxVerse << (QList<int>() << 14 << 17 << 18 << 6);
    m_maxVerse << (QList<int>() << 25 << 23 << 17 << 25 << 48 << 34 << 29 << 34 << 38 << 42 << 30 << 50 << 58 << 36 << 39 << 28 << 27 << 35 << 30 << 34 << 46 << 46 << 39 << 51 << 46 << 75 << 66 << 20);
    m_maxVerse << (QList<int>() << 45 << 28 << 35 << 41 << 43 << 56 << 37 << 38 << 50 << 52 << 33 << 44 << 37 << 72 << 47 << 20);
    m_maxVerse << (QList<int>() << 80 << 52 << 38 << 44 << 39 << 49 << 50 << 56 << 62 << 42 << 54 << 59 << 35 << 35 << 32 << 31 << 37 << 43 << 48 << 47 << 38 << 71 << 56 << 53);
    m_maxVerse << (QList<int>() << 51 << 25 << 36 << 54 << 47 << 71 << 53 << 59 << 41 << 42 << 57 << 50 << 38 << 31 << 27 << 33 << 26 << 40 << 42 << 31 << 25);
    m_maxVerse << (QList<int>() << 26 << 47 << 26 << 37 << 42 << 15 << 60 << 40 << 43 << 48 << 30 << 25 << 52 << 28 << 41 << 40 << 34 << 28 << 41 << 38 << 40 << 30 << 35 << 27 << 27 << 32 << 44 << 31);
    m_maxVerse << (QList<int>() << 32 << 29 << 31 << 25 << 21 << 23 << 25 << 39 << 33 << 21 << 36 << 21 << 14 << 23 << 33 << 27);
    m_maxVerse << (QList<int>() << 31 << 16 << 23 << 21 << 13 << 20 << 40 << 13 << 27 << 33 << 34 << 31 << 13 << 40 << 58 << 24);
    m_maxVerse << (QList<int>() << 24 << 17 << 18 << 18 << 21 << 18 << 16 << 24 << 15 << 18 << 33 << 21 << 14);
    m_maxVerse << (QList<int>() << 24 << 21 << 29 << 31 << 26 << 18);
    m_maxVerse << (QList<int>() << 23 << 22 << 21 << 32 << 33 << 24);
    m_maxVerse << (QList<int>() << 30 << 30 << 21 << 23);
    m_maxVerse << (QList<int>() << 29 << 23 << 25 << 18);
    m_maxVerse << (QList<int>() << 10 << 20 << 13 << 18 << 28);
    m_maxVerse << (QList<int>() << 12 << 17 << 18);
    m_maxVerse << (QList<int>() << 20 << 15 << 16 << 16 << 25 << 21);
    m_maxVerse << (QList<int>() << 18 << 26 << 17 << 22);
    m_maxVerse << (QList<int>() << 16 << 15 << 15);
    m_maxVerse << (QList<int>() << 25);
    m_maxVerse << (QList<int>() << 14 << 18 << 19 << 16 << 14 << 20 << 28 << 13 << 28 << 39 << 40 << 29 << 25);
    m_maxVerse << (QList<int>() << 27 << 26 << 18 << 17 << 20);
    m_maxVerse << (QList<int>() << 25 << 25 << 22 << 19 << 14);
    m_maxVerse << (QList<int>() << 21 << 22 << 18);
    m_maxVerse << (QList<int>() << 10 << 29 << 24 << 21 << 21);
    m_maxVerse << (QList<int>() << 13);
    m_maxVerse << (QList<int>() << 14);
    m_maxVerse << (QList<int>() << 25);
    m_maxVerse << (QList<int>() << 20 << 29 << 22 << 11 << 14 << 17 << 17 << 13 << 21 << 11 << 19 << 17 << 18 << 20 << 8 << 21 << 18 << 24 << 21 << 15 << 27 << 21);

}
QStringList Versification_KJV::getBookNames(VersificationFilterFlags filter) const
{
    //in kjv ALL equals to OT && NT
    if(filter.testFlag(Versification::ReturnAll) || (filter.testFlag(Versification::ReturnOT) && filter.testFlag(Versification::ReturnNT)))
        return m_bookNames;

}
QList<QStringList> Versification_KJV::multipleBookShortNames(VersificationFilterFlags filter) const
{
    if(filter.testFlag(Versification::ReturnAll) || (filter.testFlag(Versification::ReturnOT) && filter.testFlag(Versification::ReturnNT)))
        return m_bookShortNames;
}
QStringList Versification_KJV::bookShortNames(VersificationFilterFlags filter) const
{
    return QStringList();
}
QList<int> Versification_KJV::maxChapter(VersificationFilterFlags filter) const
{
    return m_maxChapter;
}
QList<QList<int> > Versification_KJV::maxVerse(VersificationFilterFlags filter) const
{
    return m_maxVerse;
}
