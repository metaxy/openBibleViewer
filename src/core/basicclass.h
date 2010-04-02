#ifndef BASICCLASS_H
#define BASICCLASS_H
#include "src/core/notes.h"
#include "src/module/modulemanager.h"
#include "src/core/settings.h"
#include "src/core/bibledisplay.h"
class BasicClass
{
public:
    BasicClass();
    ModuleManager *m_moduleManager;
    BibleDisplay *m_bibleDisplay;
    Settings *m_settings;
    Notes *m_notes;
    void setModuleManager(ModuleManager *manager);
    void setBibleDisplay(BibleDisplay *bibleDisplay);
    void setSettings(Settings *settings);
    void setNotes(Notes *notes);
};

#endif // BASICCLASS_H
