#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include "src/module/modulemanager.h"
#include "src/core/bibledisplay.h"
#include "src/core/settings.h"
#include "src/core/notes.h"
class Interface : public QWidget
{
    Q_OBJECT
public slots:
    void closing();

public:
    Interface(QWidget *parent);
    ModuleManager *m_moduleManager;
    BibleDisplay *m_bibleDisplay;
    Settings *m_settings;
    Notes *m_notes;

    void setModuleManager(ModuleManager *manager);
    void setBibleDisplay(BibleDisplay *bibledisplay);
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);

    bool hasMenuBar();
    QMenuBar* menuBar();

    bool hasToolBar();
    QToolBar* toolBar();

};

#endif // INTERFACE_H
