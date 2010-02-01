#include "simplemoduleclass.h"

SimpleModuleClass::SimpleModuleClass()
{
}
void SimpleModuleClass::setSettings(Settings *settings)
{
     m_settings = settings;
}
void SimpleModuleClass::setNotes(Notes *notes)
{
    m_notes = notes;
}
