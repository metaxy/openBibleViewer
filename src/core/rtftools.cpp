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
#include <iostream>
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
QString RtfTools::fromRVF(const QByteArray &data)
{
    DEBUG_FUNC_NAME
    RvfReader reader(data);
    return reader.toHtml();
}
bool RtfTools::isRvf(const QByteArray &data)
{
    DEBUG_FUNC_NAME
    if(data.size() > 1 && data.at(0) == '-' && data.at(1) == '8') {
        myDebug() << "yo";
        return true;
    }
    myDebug() << "no";
    return false;
}

QString RvfReader::readUntilLineEnd(const QByteArray &data)
{
    //myDebug() << "start" << data;
    char prev = 0;
    QByteArray n;
    for(int i = 0; i < data.size(); i++) {
        char c = data.at(i);
        if(c == 10 && prev == 13) break;
        n.append(c);
        prev = c;
    }
    //myDebug() << "ret" << QString::fromAscii(n);
    return QString::fromAscii(n);
}

QString RvfReader::readRecText(const QByteArray &data, int limit_pos)
{
    myDebug() << "start" << "limit pos" <<limit_pos << "pos" << m_pos;
    char prev = 0;
    int i;
    QByteArray n;
    for(i = 0; i < limit_pos; i++) {
        char c = data.at(i);
        if(c == 10 && prev == 13) break;
        n.append(c);
        prev = c;
    }
    m_pos += i;
    myDebug() << "ret" << QString::fromAscii(n);
    return QString::fromAscii(n);
}

QString RvfReader::readText(const QByteArray &data)
{
    myDebug() << "start"  << "mpos" << m_pos;
    char prev = 0;
    int i;
    QByteArray n;
   // n.append(0xff);
   // n.append(0xfe);
    for(i = 0; i < data.size(); i++) {
        if(i >= data.size() - 1) break;
        char c = data.at(i);
        if(c == 10 && prev == 13) break;

       /* if (c == 0x20 && prev == 0x29) {
            n.remove(data.size() - 1,1);
            n.append(13);
            n.append(13);
            n.append((char)0);
            n.append(10);
            n.append((char)0);
        }
        else {*/
            n.append(c);
            if(data.at(i+1) == 0) {
                i++;
            }
      /*  }*/
        prev = c;
    }
    myDebug() << "read " << i << "charachters";
    m_pos += i + 2;
    myDebug() << "ret" << QString::fromAscii(n);
    return QString::fromAscii(n);
}

QString RvfReader::readRecord(const QByteArray &data)
{
    QString ret;
    char prev = -1;
    int i;
    for(i = 0; i < data.size(); i++) {
        char c = data.at(i);
        if(c == 10 && prev == 13) break;
        ret.append(c);
        prev = c;
    }
    m_pos += i;
    myDebug() << "ret" << ret;
    return ret;
}

QByteArray RtfTools::gUncompress(const QByteArray &data)
{
    DEBUG_FUNC_NAME;
    myDebug() << data;
    if (data.size() <= 4) {
        myWarning() << "Input data is truncated";
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
            myWarning() << "need dict";
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
            myWarning() << "data error";
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            myWarning() << "mem error";
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    myDebug() << result;
    return result;
}

RvfReader::RvfReader(const QByteArray &data) : m_data(data), m_pos(0)
{

}
QString RvfReader::toHtml()
{
    /*QFile f("/home/paul/test.rvf");
    if(f.open(QFile::WriteOnly)) {
        f.write(data);
    }*/

    QList<int> recordStartPositions;
    QString ret;
    for(int i = 0; i < m_data.size(); i++) {
        QChar c = QChar(m_data.at(i));
        if((c == '-' && QChar(m_data.at(i+1)).isDigit()) || c.isDigit()) {
            QString r = readUntilLineEnd(m_data.mid(i));
            if(r.size() > 10) {
                recordStartPositions << i;
            }
            i += 14;
        }
    }
    myDebug() << "recordStartPositions = " << recordStartPositions;

    int end_pos = m_data.size() - 1;
    m_pos = 0;

    for(int i = 0; i < recordStartPositions.size(); i++) {
        m_pos = recordStartPositions.at(i);
        myDebug() << "pos = " << m_pos;
        const QByteArray current = m_data.mid(m_pos);
        myDebug() << "current = " << current;

        QString rvfRecHeaderStr = readRecord(current);

        QStringList rvfRecHeader = rvfRecHeaderStr.split(' ');
        if(rvfRecHeader.size() < 2) {
            continue;
        }
        int recHdrRecordType = rvfRecHeader.at(0).toInt();
        int recHdrStringsCount = rvfRecHeader.at(1).toInt();

        myDebug() << "type = " << recHdrRecordType;
        myDebug() << "count = " << recHdrStringsCount;

        if (recHdrRecordType >= 0) // text
        {
            myDebug() << "reading text ab" << m_pos;
            for (int j = 0; j < recHdrStringsCount; j++)
            {
                if (i + 1 < recordStartPositions.size())
                    end_pos = recordStartPositions.at(i + 1) - 1;
                else
                    end_pos = m_data.size();

                if (m_pos >= end_pos)
                    break;

                QString textdata = readText(m_data.mid(m_pos, end_pos));
                ret.append(textdata);
                ret.append("\r\n");
            }
        }
        else if (recHdrRecordType == -3) //
        {

        } else {
            myDebug() << "unkown header record type" << recHdrRecordType;
        }
    }
    myDebug() << ret.size() << ret;
    return ret;
}
