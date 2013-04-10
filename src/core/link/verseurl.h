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
#ifndef BIBLEURL_H
#define BIBLEURL_H
#include <QHash>
#include "src/core/link/verseurlrange.h"
#include "src/core/link/url.h"
class VerseUrl : public Url
{
public:
    VerseUrl();
    VerseUrl(const VerseUrlRange &range);
    VerseUrl(const QString &url);

    void addRange(const VerseUrlRange &range);
    void addRanges(const QList<VerseUrlRange> &ranges);
    void setParam(const QString &name, const QString &value);
    bool hasParam(const QString &name) const;
    QString getParam(const QString &name) const;
    void unsetParam(const QString &name);

    QString toString() const;
    bool fromStringUrl(QString url);
    bool fromMscope(const QString &url);
    bool fromTheWord(QString url);
    bool fromESword(QString url);
    QList<VerseUrlRange> ranges() const;
    void clearRanges();
    bool contains(int moduleID, int bookID, int chapterID, int verseID) const;
    bool contains(int moduleID, int bookID, int chapterID) const;
    bool isValid() const;
    /**
     * @brief See VerseUrlRange::setOpenToTransformation
     * @param open
     */
    void setOpenToTransformation(bool open);

    VerseUrl applyUrl(const VerseUrl &url);

    void setModuleID(const int moduleID);
    bool hasModuleID() const;
    int moduleID() const;

    /**
     * @brief See VerseUrlRange::removeNo
     */
    void removeNo();
private:
    QHash<QString, QString> m_params;
    QList<VerseUrlRange> m_ranges;
    bool m_isValid;
};

#endif // BIBLEURL_H
