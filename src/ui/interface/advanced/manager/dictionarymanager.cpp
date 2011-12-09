/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
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

void DictionaryManager::open(const QString &key, OBVCore::ContentType contentType)
{
    OBVCore::DefaultModule defaultModule = ModuleManager::toDefaultModule(contentType);

    //get default module id for this
    int defaultModuleID = -1;
    int contentModuleID = -1;
    int justDictModuleID = -1;
    QHashIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
    while(i.hasNext()) {
        i.next();
        if(i.value()->defaultModule == defaultModule)
            defaultModuleID = i.key();
        if(i.value()->contentType == contentType) {
            contentModuleID = i.key();
        }
    }
    QMapIterator<int, Module*> i2(m_moduleManager->m_moduleMap->data);
    while(i2.hasNext()) {
        i2.next();
        if(i2.value()->moduleClass() == OBVCore::DictionaryModuleClass)
            justDictModuleID = i2.key();
    }


    QMdiSubWindow *w = m_windowManager->hasDictWindow(defaultModule);
    DictionaryForm *f = NULL;
    if(w != NULL) {
       //we can use this form
       f = (DictionaryForm*) m_windowManager->getForm(w);
       m_windowManager->activate(w);
    } else {
        //we need a dictionary window where to open it
        w = m_windowManager->needDictionaryWindow();
        f = ((DictionaryForm*)m_windowManager->getForm(w));
    }
    if(f == NULL)
        return;
    if(f->dictionary()) {
        if(m_settings->getModuleSettings(f->dictionary()->moduleID())->contentType == contentType) {
            f->showEntry(key, -1);
            return;
        }
    }
    //else load default module for this content type

    if(defaultModuleID != -1) {
        f->showEntry(key, defaultModuleID);
        return;
    }

    if(contentModuleID != -1) {
        f->showEntry(key, contentModuleID);
        return;
    }

    if(justDictModuleID != -1) {
        f->showEntry(key, justDictModuleID);
        return;
    }
    //there is nothing we can do now


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
        open(url, OBVCore::StrongsContent);
    } else if(url.startsWith(gram)) {
        //gram://gramID
        url = url.remove(0, gram.size());
        open(url, OBVCore::GramContent);
    } else if(url.startsWith(rmac)) {
        //rmac://rmacID
        url = url.remove(0, rmac.size());
        open(url, OBVCore::RMacContent);
    } else if(url.startsWith(dict)) {
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
