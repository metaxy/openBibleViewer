#ifndef VERSIFICATIONEXTENDEDDATA_H
#define VERSIFICATIONEXTENDEDDATA_H

class VersificationExtendedData
{
public:
    VersificationExtendedData();
    void setHasChapterZeor(bool chapterZero);
    bool hasChapterZero();
private:
    bool m_hasChapterZero;
};

#endif // VERSIFICATIONEXTENDEDDATA_H
