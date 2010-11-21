#ifndef RANGES_H
#define RANGES_H
#include "src/core/bible/range.h"
#include <QtCore/QList>
class Ranges
{
public:
    Ranges();
    void addRange(Range range);
    QList<Range> getList() const;
private:
    QList<Range> m_ranges;
};

#endif // RANGES_H
