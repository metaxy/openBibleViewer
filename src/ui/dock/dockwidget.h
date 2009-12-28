#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>
#include "src/module/modulemanager.h"
#include "src/core/bibledisplay.h"
#include "src/core/settings.h"
#include "src/core/notes.h"
class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    DockWidget(QWidget *parent);
    ModuleManager *m_moduleManager;
    BibleDisplay *m_bibleDisplay;
    Settings *m_settings;
    Notes *m_notes;
    void setModuleManager(ModuleManager *manager);
    void setBibleDisplay(BibleDisplay *bibleDisplay);
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
signals:
    void get(QString url);
};

#endif // DOCKWIDGET_H
