#include "notesmanager.h"
#include "src/core/dbghelper.h"
NotesManager::NotesManager(QObject *parent) :
    QObject(parent)
{
}
void NotesManager::setWidget(QWidget *p)
{
    m_p = p;
}
void NotesManager::createDocks()
{
    m_notesDockWidget = new NotesDockWidget(m_p);
    setAll(m_notesDockWidget);
    m_notesDockWidget->init();
    m_notesDockWidget->hide();
  //  connect(m_notesDockWidget, SIGNAL(get(QString)), this, SLOT(pharseUrl(QString)));
   // connect(m_notesDockWidget, SIGNAL(reloadChapter()), this, SLOT(reloadChapter()));
}

QHash<DockWidget*, Qt::DockWidgetArea> NotesManager::docks()
{
    DEBUG_FUNC_NAME
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_notesDockWidget, Qt::RightDockWidgetArea);
    return ret;

}

void NotesManager::newCustomColorMark(VerseSelection selection, QColor color)
{
    m_notesDockWidget->newStyleMark(selection,  "font-color: "+color.name()+";");
}

void NotesManager::newBoldMark(VerseSelection selection)
{
    m_notesDockWidget->newStyleMark(selection, "font-weight: bold;");
}

void NotesManager::newItalicMark(VerseSelection selection)
{
    m_notesDockWidget->newStyleMark(selection, "font-style: italic;");

}
void NotesManager::newUnderlineMark(VerseSelection selection)
{
    m_notesDockWidget->newStyleMark(selection, "text-decoration:underline;");

}

void NotesManager::removeMark(VerseSelection selection)
{
    m_notesDockWidget->removeMark(selection);
}
void NotesManager::newNoteWithLink(VerseSelection selection)
{
    m_notesDockWidget->newNoteWithLink(selection);
}
