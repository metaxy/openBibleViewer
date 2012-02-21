#ifndef RTFTOOLS_H
#define RTFTOOLS_H
#include <QtCore/QString>

class RtfTools
{
public:
    RtfTools();

    enum TheWordContentType {
        RTFContent = 0,
        RVFContent = 1
    };

    static QString toValidRTF(QString data);
    static QString fromRVF(const QByteArray &data);
    static QByteArray gUncompress(const QByteArray &data);
};

#endif // RTFTOOLS_H
