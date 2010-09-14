#include "faststart.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include "src/core/dbghelper.h"
FastStart::FastStart()
{
    m_changed = false;
}
void FastStart::setSettings(Settings *settings)
{
    m_settings = settings;
}

bool FastStart::load()
{
    DEBUG_FUNC_NAME
    QFile file(filePath());
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> m_data;
    file.close();
    return true;
}

bool FastStart::save()
{
    QFile file(filePath());
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << m_data;
    file.close();
    return true;
}

bool FastStart::remove()
{
    QDir dir(m_settings->homePath);
    return dir.remove(filePath());
}

void FastStart::setFileNames(const QString &dir, const QStringList &files)
{
    m_data[dir] = files;
    m_changed = true;
}

QStringList FastStart::getFileNames(const QString &dir) const
{
    return m_data[dir];
}

bool FastStart::hasCache(const QString &dir) const
{
    return m_data.contains(dir);
}

bool FastStart::changed() const
{
    return m_changed;
}

QString FastStart::filePath() const
{
    return m_settings->homePath + "/faststart";
}
