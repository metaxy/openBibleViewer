#ifndef SIMPLEMODULECLASS_H
#define SIMPLEMODULECLASS_H
#include "src/core/notes.h"
#include "src/core/settings.h"
class SimpleModuleClass
{
public:
    SimpleModuleClass();
    void setSettings(Settings *settings);
    void setNotes(Notes *n);
    Settings *m_settings;
    Notes *m_notes;
private:

};

#endif // SIMPLEMODULECLASS_H
