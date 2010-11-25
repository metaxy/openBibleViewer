#include "textranges.h"

TextRanges::TextRanges()
{
}
void TextRanges::addTextRange(const TextRange &range)
{
    m_ranges.append(range);
}
QList<TextRange> TextRanges::textRanges()
{
    return m_ranges;
}
