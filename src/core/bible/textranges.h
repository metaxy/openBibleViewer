#ifndef TEXTRANGES_H
#define TEXTRANGES_H
#include <QtCore/QList>
#include <QtCore/QSet>
#include "src/core/bible/textrange.h"
class TextRanges
{
public:
    TextRanges();
    void addTextRange(const TextRange &range);
    void addTextRanges(const TextRanges &ranges);
    QList<TextRange> textRanges() const;
    QString join(const QString &seperator) const;

    QSet<int> booksIDs();
    QSet<int> chapterIDs();
private:
    QList<TextRange> m_ranges;
};

#endif // TEXTRANGES_H
