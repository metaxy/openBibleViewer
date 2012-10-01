#ifndef TEXTFRAGMENT_H
#define TEXTFRAGMENT_H
#include "rblock.h"
#include <QtCore/QString>
class TextFragment : public RBlock
{
public:
    TextFragment(quint32 id, const RMetaData& data);
    QString text;
};

#endif // TEXTFRAGMENT_H
