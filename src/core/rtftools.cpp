#include "rtftools.h"

RtfTools::RtfTools()
{
}
QString RtfTools::toValidRTF(QString data)
{
    if(!data.startsWith("{\\rtf")) {
        data.prepend("{\\rtf1 ");
        data.append(" }");
    }
    return data;
}
