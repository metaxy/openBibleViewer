#include "dictionarymanager.h"

DictionaryManager::DictionaryManager(QObject *parent) :
    QObject(parent)
{
}
void DictionaryManager::setWidget(QWidget *p)
{
    m_p = p;
}

void DictionaryManager::createDocks()
{
    m_dictionaryDock = new DictionaryDockWidget(m_p);
    setAll(m_dictionaryDock);
    m_dictionaryDock->init();
    m_dictionaryDock->hide();
}

QHash<DockWidget*, Qt::DockWidgetArea> DictionaryManager::docks()
{
    QHash<DockWidget *, Qt::DockWidgetArea> ret;
    ret.insert(m_dictionaryDock,  Qt::BottomDockWidgetArea);
    return ret;
}

DictionaryDockWidget* DictionaryManager::dictionaryDockWidget()
{
    return m_dictionaryDock;
}

void DictionaryManager::pharseUrl(QString url)
{
    const QString strong = "strong://";
    const QString dict = "dict:/";
    const QString gram = "gram://";

     if(url.startsWith(strong)) {
        //strong://strongID
        url = url.remove(0, strong.size());
        m_dictionaryDock->showEntry(url);
    } else if(url.startsWith(gram)) {
        //gram://gramID
        url = url.remove(0, gram.size());
        m_dictionaryDock->showEntry(url);
    } else if(url.startsWith(dict)) {
        //dict:/module/key
        url = url.remove(0, dict.size());
        QString tmp = url;
        QStringList l = tmp.split("/");
        const QString moduleID = l.first();
        const QString key = l.last();
        m_dictionaryDock->showEntry(url);
    }
}
