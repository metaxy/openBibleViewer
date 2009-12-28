#include "interface.h"

Interface::Interface(QWidget *parent) :
        QWidget(parent)
{
}
void Interface::setModuleManager(ModuleManager *manager)
{
    m_moduleManager = manager;
}
void Interface::setBibleDisplay(BibleDisplay *bibleDisplay)
{
    m_bibleDisplay = bibleDisplay;
}
void Interface::setSettings(Settings *settings)
{
    m_settings = settings;
}
void Interface::setNotes(Notes *notes)
{
    m_notes = notes;
}

