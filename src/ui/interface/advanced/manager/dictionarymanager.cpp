#include "dictionarymanager.h"
#include "src/ui/interface/advanced/form/dictionaryform.h"

DictionaryManager::DictionaryManager(QObject *parent) :
    QObject(parent)
{
}
void DictionaryManager::setWidget(QWidget *p)
{
    m_p = p;
}
void DictionaryManager::setWindowManager(WindowManager *windowManager)
{
    m_windowManager = windowManager;
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
    myDebug() << url;
    const QString strong = "strong://";
    const QString dict = "dict:/";
    const QString gram = "gram://";

     if(url.startsWith(strong)) {
        //strong://strongID
        url = url.remove(0, strong.size());
        QMdiSubWindow *w = m_windowManager->hasDictWindow(ModuleSettings::DefaultStrongDictModule);
        if(w) {
             ((DictionaryForm*)m_windowManager->getForm(w))->showEntry(url, -1);
        } else {
            int moduleID = -1;
            QHashIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
            while (i.hasNext()) {
                i.next();
                if(i.value()->defaultModule == ModuleSettings::DefaultStrongDictModule)
                    moduleID = i.key();
            }
            QMdiSubWindow *w = m_windowManager->needDictionaryWindow();
            DictionaryForm* d = ((DictionaryForm*)m_windowManager->getForm(w));
            if(d->dictionary() != NULL) {
                d->showEntry(url, -1);
            } else {
                if(moduleID == -1) {
                    QMapIterator<int, Module*> i(m_moduleManager->m_moduleMap->m_map);
                    while (i.hasNext()) {
                        i.next();
                        if(i.value()->moduleClass() == OBVCore::DictionaryModuleClass)
                            moduleID = i.key();
                    }

                    if(moduleID != -1)
                        d->showEntry(url, moduleID);
                } else {
                    d->showEntry(url, moduleID);
                }
            }
        }
    } else if(url.startsWith(gram)) {
        //gram://gramID
        url = url.remove(0, gram.size());
        m_dictionaryDock->showEntry(url);
    } else if(url.startsWith(dict)) {
        //dict:/module/key
        m_windowManager->needDictionaryWindow();

        url = url.remove(0, dict.size());
        QString tmp = url;
        QStringList l = tmp.split("/");

        QString moduleID = "";
        QString key = "";

        if(l.size() == 1) {
            moduleID = tmp;
        } else {
            moduleID = l.first();
            key = l.last();
        }
        int imoduleID;

        if(moduleID == "current") {
            imoduleID = -1;
        } else {
            imoduleID = moduleID.toInt();
        }
        ((DictionaryForm*)m_windowManager->activeForm())->showEntry(key, imoduleID);
    }
}
