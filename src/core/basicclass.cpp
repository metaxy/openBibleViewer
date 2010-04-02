#include "basicclass.h"

BasicClass::BasicClass()
{
}
void BasicClass::setModuleManager(ModuleManager *manager)
{
    m_moduleManager = manager;
}
void BasicClass::setBibleDisplay(BibleDisplay *bibleDisplay)
{
    m_bibleDisplay = bibleDisplay;
}
void BasicClass::setSettings(Settings *settings)
{
    m_settings = settings;
}
void BasicClass::setNotes(Notes *notes)
{
    m_notes = notes;
}
