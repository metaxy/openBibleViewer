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

void DictionaryManager::open(const QString &key, ModuleSettings::DefaultModule defaultModule)
{
    QMdiSubWindow *w = m_windowManager->hasDictWindow(defaultModule);
    if(w != NULL) {
         ((DictionaryForm*)m_windowManager->getForm(w))->showEntry(key, -1);
    } else {
        int moduleID = -1;
        QHashIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
        while (i.hasNext()) {
            i.next();
            if(i.value()->defaultModule == defaultModule)
                moduleID = i.key();
        }

        QMdiSubWindow *w = m_windowManager->needDictionaryWindow();
        DictionaryForm* d = ((DictionaryForm*)m_windowManager->getForm(w));
        if(d->dictionary() != NULL) {
            d->showEntry(key, -1);
        } else {
            if(moduleID != -1) {
                d->showEntry(key, moduleID);
            } else {
                QMapIterator<int, Module*> i(m_moduleManager->m_moduleMap->m_map);
                while (i.hasNext()) {
                    i.next();
                    if(i.value()->moduleClass() == OBVCore::DictionaryModuleClass)
                        moduleID = i.key();
                }

                if(moduleID != -1) {
                    d->showEntry(key, moduleID);
                }
            }
        }
    }
}

void DictionaryManager::pharseUrl(QString url)
{
    myDebug() << url;
    const QString strong = "strong://";
    const QString dict = "dict:/";
    const QString gram = "gram://";
    const QString rmac = "rmac://";

     if(url.startsWith(strong)) {
        //strong://strongID
        url = url.remove(0, strong.size());
        open(url, ModuleSettings::DefaultStrongDictModule);
    } else if(url.startsWith(gram)) {
        //gram://gramID
        url = url.remove(0, gram.size());
        open(url, ModuleSettings::DefaultGramDictModule);
    } else if(url.startsWith(rmac)) {
         //rmac://rmacID
         url = url.remove(0, rmac.size());
         open(url, ModuleSettings::DefaultRMACDictModule);
     }else if(url.startsWith(dict)) {
        //dict:/module/key
        m_windowManager->needDictionaryWindow();

        url = url.remove(0, dict.size());
        const QStringList l = url.split("/");

        QString moduleID = "";
        QString key = "";

        if(l.size() == 1) {
            moduleID = url;
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
