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
    static bool isRvf(const QByteArray &data);
    static QByteArray gUncompress(const QByteArray &data);
private:

};
class RvfReader {
public:
    RvfReader(const QByteArray &data);
    QString toHtml();
private:
    QString readRecText(const QByteArray &data, int limit_pos);
    QString readText(const QByteArray &data);
    QString readRecord(const QByteArray &data);
    QString readUntilLineEnd(const QByteArray &data);
    QByteArray m_data;
    int m_pos;

};

#endif // RTFTOOLS_H
