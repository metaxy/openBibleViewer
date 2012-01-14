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
#include "src/core/link/strongurl.h"
DictionaryManager::DictionaryManager(QObject *parent, QWidget *p) :
    QObject(parent), m_p(p)
{
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

void DictionaryManager::open(const QString &key, OBVCore::ContentType contentType, const Actions::OpenLinkModifiers mod, const QString &url)
{
    OBVCore::DefaultModule defaultModule = ModuleManager::toDefaultModule(contentType);
    myDebug() << "content = " << contentType << " so the defaultModule is" << defaultModule;
    //get default module id for this
    int defaultModuleID = -1;
    int contentModuleID = -1;
    int justDictModuleID = -1;
    QMapIterator<int, ModuleSettings*> i(m_settings->m_moduleSettings);
    while(i.hasNext()) {
        i.next();
        if(i.value()->defaultModule == defaultModule)
            defaultModuleID = i.key();
        if(ModuleManager::alsoOk(i.value()->contentType, contentType)) {
            contentModuleID = i.key();
        }
    }
    QMapIterator<int, Module*> i2(m_moduleManager->m_moduleMap->data);
    while(i2.hasNext()) {
        i2.next();
        if(i2.value()->moduleClass() == OBVCore::DictionaryModuleClass)
            justDictModuleID = i2.key();
    }

    myDebug() << defaultModuleID << contentModuleID << justDictModuleID;
    DictionaryForm *f = NULL;
    QMdiSubWindow *w = m_windowManager->hasDictWindow(defaultModule);


    if(mod == Actions::OpenInNewWindow) {
        //we will open a new window anyway
        w = m_windowManager->newDictionarySubWindow();
        f = (DictionaryForm*) m_windowManager->getForm(w);
        m_windowManager->activate(w);
    } else {
        if(w) {
           f = (DictionaryForm*) m_windowManager->getForm(w);
           OBVCore::ContentType type = m_windowManager->contentType(f);

           if(ModuleManager::alsoOk(type,contentType) || type == OBVCore::UnkownContent) {
                //we can use this form
                m_windowManager->activate(w);
           } else {
                //we will find a better one
                w = m_windowManager->needDictionaryWindow(contentType);
                f = ((DictionaryForm*)m_windowManager->getForm(w));
           }

        } else {
            //we need a new dictionary window, because there are no dictionary windows
            w = m_windowManager->needDictionaryWindow();
            f = ((DictionaryForm*)m_windowManager->getForm(w));
        }
    }
    //something gone wrong
    if(f == NULL)
        return;
    //we could reuse thios module
    if(f->dictionary()) {
        myDebug() << "has already a dictionary";
        OBVCore::ContentType type = m_windowManager->contentType(f);
        myDebug() << type << " mine is " << contentType;
        if(ModuleManager::alsoOk(type,contentType) || type == OBVCore::UnkownContent) {
            f->historySetUrl(url);
            f->showEntry(key, -1);
            return;
        }
    }
    //else load default module for this content type

    if(defaultModuleID != -1) {
        f->historySetUrl(url);
        f->showEntry(key, defaultModuleID);
        return;
    }

    if(contentModuleID != -1) {
        f->historySetUrl(url);
        f->showEntry(key, contentModuleID);
        return;
    }

    if(justDictModuleID != -1) {
        f->historySetUrl(url);
        f->showEntry(key, justDictModuleID);
        return;
    }
    //there is nothing we can do now
}

void DictionaryManager::pharseUrl(QString url, const Actions::OpenLinkModifiers mod)
{
    myDebug() << url;
    const QString backupUrl = url;
    const QString dict = "dict:/";
    if(url.startsWith(OBVCore::strongScheme)) {
        StrongUrl strong;
        strong.fromString(url);
        if(strong.prefix() == StrongUrl::G) {
            open(strong.toKey(), OBVCore::StrongsGreekContent, mod, backupUrl);
        } else if(strong.prefix() == StrongUrl::H){
            open(strong.toKey(), OBVCore::StrongsHebrewContent, mod, backupUrl);
        }
    } else if(url.startsWith(OBVCore::gramScheme)) {
        //gram://gramID

        url = url.remove(0, OBVCore::gramScheme.size());
        open(url, OBVCore::GramContent, mod, backupUrl);

    } else if(url.startsWith(OBVCore::rmacScheme)) {
        //rmac://rmacID

        url = url.remove(0, OBVCore::rmacScheme.size());
        open(url, OBVCore::RMacContent, mod, backupUrl);

    } else if(url.startsWith(dict)) {
        //dict:/module/key
        QMdiSubWindow *window = NULL;
        if(mod == Actions::OpenInNewWindow) {
            window = m_windowManager->newDictionarySubWindow();
        } else {
            window = m_windowManager->needDictionaryWindow();
        }

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
        DictionaryForm *f = ((DictionaryForm*)m_windowManager->getForm(window));
        f->historySetUrl(url);
        f->showEntry(key, imoduleID);
    }
}
