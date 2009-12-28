#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QSettings>
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


protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
