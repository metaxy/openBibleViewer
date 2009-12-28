#include "dockwidget.h"

DockWidget::DockWidget(QWidget *parent) :
        QDockWidget(parent)
{
}
void DockWidget::setModuleManager(ModuleManager *manager)
{
    m_moduleManager = manager;
}
void DockWidget::setBibleDisplay(BibleDisplay *bibleDisplay)
{
    m_bibleDisplay = bibleDisplay;
}
void DockWidget::setSettings(Settings *settings)
{
    m_settings = settings;
}
void DockWidget::setNotes(Notes *notes)
{
    m_notes = notes;
}

