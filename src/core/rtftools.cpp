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
    QByteArray n;
    int i;
    for(i = 0; i < data.size() - 1; i++) { // - 1 because we have to look one forward in the if for the break
        char c = data.at(i);
      //  myDebug() << "i=" << i << "c=" << (int)c;
        if(c == 0x0D/*13*/ && data.at(i+1) == 0x0A/*10*/) break;
        n.append(c);
    }
    myDebug() << "ret=" << QString::fromAscii(n).size() << "i="<< i;
    return QString::fromAscii(n);
}

QString RvfReader::readRecText(const QByteArray &data, int limit_pos)
{
    myDebug() << "start" << "limit pos" <<limit_pos << "mpos" << m_pos;
    char prev = 0;
    int i;
    QByteArray n;
    for(i = 0; i < limit_pos; i++) {
        char c = data.at(i);
        if(c == 10 && prev == 13) break;
        n.append(c);
        prev = c;
    }
    m_pos += i + 1;
    //myDebug() << "ret" << QString::fromAscii(n);
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

        if(c == 41) {
            n.append("<br />");
            i++;
        } else {
            n.append(c);

            if(data.at(i+1) == 0) {
                i++;
            }
        }

        prev = c;
    }
    //myDebug() << "read " << i << "charachters";
    m_pos += i + 1;
    //myDebug() << "ret" << QString::fromAscii(n);
    return QString::fromAscii(n);
}

QString RvfReader::readRecord(const QByteArray &data)
{
    myDebug() << "mpos=" << m_pos;
    QString ret;
    char prev = -1;
    int i;
    for(i = 0; i < data.size(); i++) {
        char c = data.at(i);
        if(c == 10 && prev == 13) break;

        ret.append(c);
        prev = c;
    }
    m_pos += i + 1;
    myDebug() << "ret" << ret;
    return ret;
}

QByteArray RtfTools::gUncompress(const QByteArray &data)
{
    DEBUG_FUNC_NAME;
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
    return result;

}
class TItem
{
public:
    enum TType {
        Text,
        Link,
        H1,
        H2
    };

    ~TItem() {
        foreach(TItem *x,children) {
            delete x;
        };
        children.clear();
    }
    TItem::TType ttype;
    QList<TItem*> children;
    int type;
    QString text;

};

RvfReader::RvfReader(const QByteArray &data) : m_data(data), m_pos(0)
{

}
QList<int> RvfReader::startPositions()
{
    QList<int> recordStartPositions;
    //the first one is alway a block
    int i = 0;
    i += readUntilLineEnd(m_data).size();

    for(; i < m_data.size() - 1; i++) {
        QChar c = QChar(m_data.at(i));
      //  myDebug() <<  "i=" << i << "c=" << (int)c.toAscii();
        if((QChar(m_data.at(i+1)).isDigit() && c == '-') //is a digit with a minus before
            || (c.isDigit() && QChar(m_data.at(i-1)).isSpace())) {
            QString r = readUntilLineEnd(m_data.mid(i));
            recordStartPositions << i;
            myDebug() << "i=" << i << "r.size()=" << r.size();
            i += r.size() + 2;
        }
    }

    myDebug() << "recordStartPositions = " << recordStartPositions;
    return recordStartPositions;
}

QString RvfReader::toHtml()
{
    QFile file("out.b");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream o(&file);
    o << m_data;
    file.close();

    QString ret;

    QList<int> recordStartPositions = startPositions();
    myDebug() << "all text" << readText(m_data);

    int end_pos = m_data.size() - 1;
    m_pos = 0;

    for(int i = 0; i < recordStartPositions.size(); i++) {
        m_pos = recordStartPositions.at(i);
        myDebug() << "i=" << i << "pos=" << m_pos;
        const QByteArray current = m_data.mid(m_pos);

        QString rvfRecHeaderStr = readRecord(current);

        QStringList rvfRecHeader = rvfRecHeaderStr.split(' ');
        myDebug() << rvfRecHeader;
        if(rvfRecHeader.size() < 2)  {
            myDebug() << "cannot parse header" << rvfRecHeaderStr;
            myDebug() << "!!! text" << readText(m_data.mid(m_pos, end_pos));
            continue;
        }
        int recHdrRecordType = rvfRecHeader.at(0).toInt();
        int recHdrStringsCount = rvfRecHeader.at(1).toInt();

        myDebug() << "type=" << recHdrRecordType;
        myDebug() << "count=" << recHdrStringsCount;

        // Update end_pos
        if (i + 1 < recordStartPositions.size()) {
            myDebug() << "next one" << recordStartPositions.at(i + 1) - 1 << "m_pos" << m_pos;
            end_pos = recordStartPositions.at(i + 1) - 1 - m_pos;
        } else {
            end_pos = m_data.size() - m_pos;
        }
        if(recHdrRecordType == 10) { // h1
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append(textdata);
        } else if(recHdrRecordType == 11) { // h2
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append(textdata);
        } else if(recHdrRecordType == 12) {
            QString textdata = readText(m_data.mid(m_pos, end_pos));//wahrscheinlich textfarbe
            myDebug() << "link" << textdata << rvfRecHeader;
            ret.append("<i>" + textdata + "</i>");
        } else if(recHdrRecordType == 13) { // h2
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append("<span style='color:#B300FF'>" + textdata + "</span>");//lila
         }else if(recHdrRecordType == 14) { // h2
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append("<span style='color:#0019FF'>" + textdata + "</span>");//blau
        } else if(recHdrRecordType == 15) { // h2
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append("<span style='color:#00F3FF'>" + textdata + "</span>");//cyan
        } else if(recHdrRecordType == 16) { // link kinda
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append("<a href=" + rvfRecHeader.at(5) +">" + textdata + "</a>");//link
        } else if(recHdrRecordType == 17) { // h2
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append("<span style='color:#B7FF00'>" + textdata + "</span>");//hellgrün
        } else if (recHdrRecordType == 5) { // link!!!
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            ret.append("<a href="+ rvfRecHeader.at(5) + ">" + textdata + "</a>");
        } else if (recHdrRecordType >= 0) { // text
            myDebug() << "reading text ab" << m_pos;
            myDebug() << "m_pos" << m_pos << "end_pos" << end_pos << "i" << i;
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            myDebug() "text = " << textdata;
            ret.append(textdata);
        } else if (recHdrRecordType == -3) {
            myDebug() << "images not supported";
        } else if (recHdrRecordType == -9) {
             QString textdata = readText(m_data.mid(m_pos, end_pos));
             myDebug() << "unneeded data" << textdata;
        } else {
            QString textdata = readText(m_data.mid(m_pos, end_pos));
            myDebug() << "unkown header record type" << recHdrRecordType << textdata;
        }
        myDebug() << "----------------------------------------";
    }
    //myDebug() << ret.size() << ret;
    return ret;
}
