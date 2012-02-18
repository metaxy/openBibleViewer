#ifndef RTFTOOLS_H
#define RTFTOOLS_H
#include <QtCore/QString>

class RtfTools
{
public:
    RtfTools();
    static QString toValidRTF(QString data);
};

#endif // RTFTOOLS_H
