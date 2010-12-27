#include "versification_kjv.h"

Versification_KJV::Versification_KJV()
{
    m_bookNames << tr("Genesis");
    m_bookShortNames.append(tr("Gen, Ge, Gn").split(", "));

    m_bookNames << tr("Exodus");
    m_bookShortNames.append(tr("Ex, Exo, Exd").split(", "));
    m_bookNames << tr("Leviticus");
    m_bookShortNames.append(tr("Lev, Le, Lv").split(", "));

    m_bookNames << tr("Numbers");
    m_bookShortNames.append(tr("Num, Nu, Nm, Nb").split(", "));

    m_bookNames << tr("Deuteronomy");
    m_bookShortNames.append(tr("Deut, Dt").split(", "));

    m_bookNames << tr("Joshua");
    m_bookShortNames.append(tr("Josh, Jos, Jsh").split(", "));

    m_bookNames << tr("Judges");
    m_bookShortNames.append(tr("Judg, Jdg, Jg, Jdgs").split(", "));

    m_bookNames << tr("Ruth");
    m_bookShortNames.append(tr("Rth, Ru").split(", "));

    m_bookNames << tr("1 Samuel");
    m_bookShortNames.append(tr("1 Sam, 1 Sa, 1Samuel, 1S, I Sa, 1 Sm, 1Sa, I Sam, 1Sam").split(", "));

    m_bookNames << tr("2 Samuel");
    m_bookShortNames.append(tr("2 Sam, 2 Sa, 2S, II Sa, 2 Sm, 2Sa, II Sam, 2Sam").split(", "));

    m_bookNames << tr("1 Kings");
    m_bookShortNames.append(tr("1 Kgs, 1 Ki, 1K, I Kgs, 1Kgs, I Ki, 1Ki, I Kings, 1Kings, 1st Kgs, 1st Kings, 1Kin").split(", "));

    m_bookNames << tr("2 Kings");
    m_bookShortNames.append(tr("2 Kgs, 2 Ki, 2K, II Kgs, 2Kgs, II Ki, 2Ki, II Kings, 2Kings, 2nd Kgs, 2nd Kings, 2Kin").split(", "));

    m_bookNames << tr("1 Chronicles");
    m_bookShortNames.append(tr("1 Chron, 1 Ch, I Ch, 1Ch, 1 Chr, I Chr, 1Chr, I Chron, 1Chron").split(", "));

    m_bookNames << tr("2 Chronicles");
    m_bookShortNames.append(tr("2 Chron, 2 Ch, II Ch, 2Ch, II Chr, 2Chr, II Chron, 2Chron").split(", "));

    m_bookNames << tr("Ezra");

    m_bookShortNames.append(tr("Ezra, Ezr").split(", "));

    m_bookNames << tr("Nehemiah");
    m_bookShortNames.append(tr("Neh, Ne").split(", "));

    m_bookNames << tr("Esther");
    m_bookShortNames.append(tr("Esth, Es").split(", "));

    m_bookNames << tr("Job");
    m_bookShortNames.append(tr("Job, Job, Jb").split(", "));

    m_bookNames << tr("Psalm");
    m_bookShortNames.append(tr("Pslm, Ps, Psalms, Psa, Psm, Pss").split(", "));

    m_bookNames << tr("Proverbs");
    m_bookShortNames.append(tr("Prov, Pr, Prv").split(", "));

    m_bookNames << tr("Ecclesiastes");
    m_bookShortNames.append(tr("Eccles, Ec, Qoh, Qoheleth").split(", "));

    m_bookNames << tr("Song of Solomon");
    m_bookShortNames.append(tr("Song, So, SOS").split(", "));

    m_bookNames << tr("Isaiah");
    m_bookShortNames.append(tr("Isa, Is").split(", "));

    m_bookNames << tr("Jeremiah");
    m_bookShortNames.append(tr("Jer, Je, Jr").split(", "));

    m_bookNames << tr("Lamentations");
    m_bookShortNames.append(tr("Lam, La").split(", "));

    m_bookNames << tr("Ezekiel");
    m_bookShortNames.append(tr("Ezek, Eze, Ezk").split(", "));

    m_bookNames << tr("Daniel");
    m_bookShortNames.append(tr("Dan, Da, Dn").split(", "));

    m_bookNames << tr("Hosea");
    m_bookShortNames.append(tr("Hos, Ho").split(", "));

    m_bookNames << tr("Joel");
    m_bookShortNames.append(tr("Joel, Joe, Jl").split(", "));

    m_bookNames << tr("Amos");
    m_bookShortNames.append(tr("Amos, Am").split(", "));

    m_bookNames << tr("Obadiah");
    m_bookShortNames.append(tr("Obad, Ob").split(", "));

    m_bookNames << tr("Jonah");
    m_bookShortNames.append(tr("Jnh, Jon").split(", "));

    m_bookNames << tr("Micah");
    m_bookShortNames.append(tr("Micah, Mic").split(", "));

    m_bookNames << tr("Nahum");
    m_bookShortNames.append(tr("Nah, Na").split(", "));

    m_bookNames << tr("Habakkuk");
    m_bookShortNames.append(tr("Hab, Hab").split(", "));

    m_bookNames << tr("Zephaniah");
    m_bookShortNames.append(tr("Zeph, Zep, Zp").split(", "));

    m_bookNames << tr("Haggai");
    m_bookShortNames.append(tr("Haggai, Hag, Hg").split(", "));

    m_bookNames << tr("Zechariah");
    m_bookShortNames.append(tr("Zech, Zec, Zc").split(", "));

    m_bookNames << tr("Malachi");
    m_bookShortNames.append(tr("Mal, Mal, Ml").split(", "));

    m_bookNames << tr("Matthew");
    m_bookShortNames.append(tr("Matt, Mt").split(", "));

    m_bookNames << tr("Mark");
    m_bookShortNames.append(tr("Mrk, Mk, Mr").split(", "));

    m_bookNames << tr("Luke");
    m_bookShortNames.append(tr("Luk, Lk").split(", "));

    m_bookNames << tr("John");
    m_bookShortNames.append(tr("John, Jn, Jhn").split(", "));

    m_bookNames << tr("Acts");
    m_bookShortNames.append(tr("Acts, Ac").split(", "));

    m_bookNames << tr("Romans");
    m_bookShortNames.append(tr("Rom, Ro, Rm").split(", "));

    m_bookNames << tr("1 Corinthians");
    m_bookShortNames.append(tr("1 Cor, 1 Co, I Co, 1Co, I Cor, 1Cor").split(", "));

    m_bookNames << tr("2 Corinthians");
    m_bookShortNames.append(tr("2 Cor, 2 Co, II Co, 2Co, II Cor, 2Cor").split(", "));

    m_bookNames << tr("Galatians");
    m_bookShortNames.append(tr("Gal, Ga").split(", "));

    m_bookNames << tr("Ephesians");
    m_bookShortNames.append(tr("Ephes, Eph").split(", "));

    m_bookNames << tr("Philippians");
    m_bookShortNames.append(tr("Phil, Php").split(", "));

    m_bookNames << tr("Colossians");
    m_bookShortNames.append(tr("Col, Col").split(", "));

    m_bookNames << tr("1 Thessalonians");
    m_bookShortNames.append(tr("1 Thess, 1 Th, I Th, 1Th, I Thes, 1Thes, I Thess, 1Thess").split(", "));

    m_bookNames << tr("2 Thessalonians");
    m_bookShortNames.append(tr("2 Thess, 2 Th, II Th, 2Th, II Thes, 2Thes, II Thess, 2Thess").split(", "));

    m_bookNames << tr("1 Timothy");
    m_bookShortNames.append(tr("1 Tim, 1 Ti, I Ti, 1Ti, I Tim, 1Tim").split(", "));

    m_bookNames << tr("2 Timothy");
    m_bookShortNames.append(tr("2 Tim, 2 Ti, II Ti, 2Ti, II Tim, 2Tim").split(", "));

    m_bookNames << tr("Titus");
    m_bookShortNames.append(tr("Titus, Tit").split(", "));

    m_bookNames << tr("Philemon");
    m_bookShortNames.append(tr("Philem, Phm").split(", "));

    m_bookNames << tr("Hebrews");
    m_bookShortNames.append(tr("Hebrews, Heb").split(", "));

    m_bookNames << tr("James");
    m_bookShortNames.append(tr("James, Jas, Jm").split(", "));

    m_bookNames << tr("1 Peter");
    m_bookShortNames.append(tr("1 Pet, 1 Pe, I Pe, 1Pe, I Pet, 1Pet, I Pt, 1 Pt, 1Pt").split(", "));

    m_bookNames << tr("2 Peter");
    m_bookShortNames.append(tr("2 Pet, 2 Pe, II Pe, 2Pe, II Pet, 2Pet, II Pt, 2 Pt, 2Pt").split(", "));

    m_bookNames << tr("1 John");
    m_bookShortNames.append(tr("1 John, 1 Jn, I Jn, 1Jn, I Jo, 1Jo, I Joh, 1Joh, I Jhn, 1 Jhn, 1Jhn, I John, 1John").split(", "));

    m_bookNames << tr("2 John");
    m_bookShortNames.append(tr("2 John, 2 Jn, II Jn, 2Jn, II Jo, 2Jo, II Joh, 2Joh, II Jhn, 2 Jhn, 2Jhn, II John, 2John").split(", "));

    m_bookNames << tr("3 John");
    m_bookShortNames.append(tr("3 John, 3 Jn, III Jn, 3Jn, III Jo, 3Jo, III Joh, 3Joh, III Jhn, 3 Jhn, 3Jhn, III John, 3John").split(", "));

    m_bookNames << tr("Jude");
    m_bookShortNames.append(tr("Jude, Jud").split(", "));

    m_bookNames << tr("Revelation");
    m_bookShortNames.append(tr("Rev, Re").split(", "));







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
