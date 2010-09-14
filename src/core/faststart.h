#ifndef FASTSTART_H
#define FASTSTART_H
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include "src/core/settings.h"

class FastStart
{
public:
    FastStart();
    void setSettings(Settings *settings);
    bool load();
    bool save();
    bool remove();
    void setFileNames(const QString &dir,const QStringList &files);
    QStringList getFileNames(const QString &dir) const;
    bool hasCache(const QString &dir) const ;
    bool changed() const;
private:
    QHash<QString, QStringList> m_data;
    bool m_changed;
    QString filePath() const;
    Settings *m_settings;
};

#endif // FASTSTART_H
