#ifndef CONTEXT_H
#define CONTEXT_H
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QTranslator>

#include "src/core/basicclass.h"

class Context : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit  Context(QObject *parent);
    ~Context();

    void init();

    void setSettings(QSettings *settings);
    void setHomePath(const QString &homePath);
    void setTranslator(QTranslator *my, QTranslator *qt);

    void showWindow();

    void writeSettings();

private:
    void loadDefaultSettings();
    void loadSettings();

private:
    QSettings *m_settingsFile;
    QString m_homeDataPath;
    QTranslator *m_myappTranslator;
    QTranslator *m_qtTranslator;
    QSettings *m_sessionFile;

    bool m_firstStart;
};

#endif // CONTEXT_H
