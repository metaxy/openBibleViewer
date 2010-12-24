#ifndef THEWORDBIBLE_H
#define THEWORDBIBLE_H
#include "src/module/biblemodule.h"
class TheWordBible: public BibleModule
{
public:
    TheWordBible();
    ~TheWordBible();
    void setSettings(Settings *settings);
    void loadBibleData(const int &id, const QString &path);

    int moduleID() const;


private:
    Book m_book;
    QString indexPath() const;
    int m_moduleID;

    bool hasNT() const;
    bool hasOT() const;


};

#endif // THEWORDBIBLE_H
