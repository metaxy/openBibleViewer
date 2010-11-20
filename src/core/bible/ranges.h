#ifndef RANGES_H
#define RANGES_H
#include "src/core/bible/range.h"
#include <QtCore/QList>
class Ranges
{
public:
    Ranges();
    void addRange(Range range);
private:
    QList<Range> m_ranges;
};

#endif // RANGES_H
