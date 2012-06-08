/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "rtftools.h"
#include "src/core/dbghelper.h"
#include "zlib.h"
#include <QFile>
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
    QFile f("/home/paul/test.rvf");
    if(f.open(QFile::WriteOnly)) {
        f.write(data);
    }

    QString ret;
    myDebug() << data;
    bool show = false;
    for (int i = 0; i < data.size()-1; ++i) {
        //myDebug() << i << (int) data.at(i);
        if(data.at(i) == 0) {
            show = true;
            continue;
        }
        if(data.at(i) == 32) {
            ret += " ";
        }
        if(data.at(i) > 0x20 && data.at(i+1) == 0) {
            ret += QString(data.at(i));
        }

        show = false;
     }
    myDebug() << "ret = " << ret;
    return ret;
}
QByteArray RtfTools::gUncompress(const QByteArray &data)
{
    DEBUG_FUNC_NAME
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
    if (ret != Z_OK) {
        myWarning() << "not gzip";
        return QByteArray();
    }

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
    myDebug() << result;
    return result;
}
