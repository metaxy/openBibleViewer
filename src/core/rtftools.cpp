#include "rtftools.h"
#include "src/core/dbghelper.h"
#include "zlib.h"
RtfTools::RtfTools()
{
}
QString RtfTools::toValidRTF(QString data)
{
    if(!data.startsWith("{\\rtf")) {
        data.prepend("{\\rtf1 ");
        data.append(" }");
    }
    myDebug() << data;
    return data;
}
QString RtfTools::fromRVF(const QByteArray &data)
{
    QString ret;
    myDebug() << data;
    for (int i = 0; i < data.size(); ++i) {
        //myDebug() << i << (int) data.at(i);
        if(data.at(i) == 0) {
            continue;
        }
        if(data.at(i) == 32) {
            ret += " ";
        }
        if(data.at(i) > 68) {
            ret += QString(data.at(i));
        }
     }
    myDebug() << "ret = " << ret;
    return ret;
}
QByteArray RtfTools::gUncompress(const QByteArray &data)
{
    if (data.size() <= 4) {
        qWarning("gUncompress: Input data is truncated");
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}
