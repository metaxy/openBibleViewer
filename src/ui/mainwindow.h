#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QSettings>
#include <QtCore/QTranslator>
#include "src/module/modulemanager.h"
#include "src/core/bibledisplay.h"
#include "src/core/settings.h"
#include "src/core/notes.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void settingsChanged(Settings settings);
public slots:
    void showSettingsDialog_Module();
    void showSettingsDialog_General();
     void saveSettings(Settings set);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init(const QString &homeDataPath);
    void loadSimpleInterface();
    void loadAdvancedInterface();
    void loadStudyInterface();
    void loadDefaultSettings();
    void loadSettings();
    void restoreSession();
    ModuleManager *m_moduleManager;
    BibleDisplay *m_bibleDisplay;
    Settings *m_settings;
    Notes *m_notes;
    QSettings *m_settingsFile;
    QString VERSION, BUILD;
    QString m_homeDataPath;
    void setSettings(Settings *setttings);
    void setSettings(Settings settings);
    void writeSettings();

    void showSettingsDialog(int tabID);
    void loadLanguage(QString language);
    void setTranslator(QTranslator *my, QTranslator *qt);
    QTranslator *myappTranslator;
    QTranslator *qtTranslator;


protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
