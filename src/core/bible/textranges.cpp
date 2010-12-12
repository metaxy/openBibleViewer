#include "textranges.h"

TextRanges::TextRanges()
{
}
QString TextRanges::join(const QString &seperator) const
{
    QString out;
    foreach(const TextRange & r, m_ranges) {
        out += r.join(seperator);
    }
    return out;
}
void TextRanges::addTextRange(const TextRange &range)
{
    m_ranges.append(range);
}
void TextRanges::addTextRanges(const TextRanges &ranges)
{
    m_ranges.append(ranges.textRanges());
}

QList<TextRange> TextRanges::textRanges() const
{
    return m_ranges;
}
