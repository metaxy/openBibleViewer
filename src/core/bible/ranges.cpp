#include "ranges.h"

Ranges::Ranges()
{
}
void Ranges::addRange(Range range)
{
    m_ranges << range;
}
 QList<Range> Ranges::getList() const
 {
     return m_ranges;
 }
