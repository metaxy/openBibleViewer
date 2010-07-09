#include "bibleapi.h"
#include "src/core/dbghelper.h"
#include <QWebElementCollection>
BibleApi::BibleApi()
{
}
void BibleApi::activateBible(int bibleListID)
{
    Q_UNUSED(bibleListID);
    DEBUG_FUNC_NAME
    m_moduleManager->bibleList()->m_currentBible = bibleListID;
    QWebElementCollection collection = m_frame->documentElement().findAll("td[class~=bibleListTitle]");
    foreach (QWebElement paraElement, collection) {
        paraElement.removeClass("active");
        if(paraElement.attribute("bibleListID") == QString::number(bibleListID)) {
            paraElement.addClass("active");
        }
    }
    m_bibleDisplay->emitGet("bible://reloadActive");
}
void BibleApi::setFrame(QWebFrame *frame)
{
    m_frame = frame;
}
