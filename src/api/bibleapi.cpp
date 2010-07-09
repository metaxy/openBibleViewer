#include "bibleapi.h"
#include "src/core/dbghelper.h"
#include <QWebElementCollection>
BibleApi::BibleApi()
{
}
void BibleApi::activateBible(int moduleID)
{
    DEBUG_FUNC_NAME
    int id = 0;
    QHashIterator<int,Bible *> i(m_moduleManager->bibleList()->m_bibles);
    while(i.hasNext()) {
        i.next();
        myDebug() << i.key() << i.value()->moduleID();
        if(i.value() && i.value()->moduleID() == moduleID /*&& i.key() != id*/ /* todo: that is a bit hacky*/) {
            id = i.key();
        }
    }
    m_moduleManager->bibleList()->m_currentBible = id;
    myDebug() << id;
    QWebElementCollection collection = m_frame->documentElement().findAll("td[class~=bibleListTitle]");
     foreach (QWebElement paraElement, collection) {
         paraElement.removeClass("active");
         if(paraElement.attribute("titleOf") == QString::number(moduleID)) {
             paraElement.addClass("active");
         }
    }
    m_bibleDisplay->emitGet("bible://reloadActive");
}
void BibleApi::setFrame(QWebFrame *frame)
{
    m_frame = frame;
}
