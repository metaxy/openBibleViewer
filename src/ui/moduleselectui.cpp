#include "moduleselectui.h"
#include "src/core/dbghelper.h"
#include "src/ui/dialog/moduleconfigdialog.h"
#include "src/module/moduletools.h"
#include "src/core/link/verseurl.h"
#include <QPointer>
#include <QMenu>
ModuleSelectUI::ModuleSelectUI(BasicClass *parent, QWidget *parentWidget) : QObject(parentWidget)
{
    copy(parent);
    m_moduleID = -1;
    ModuleModel model(parentWidget);
    model.setSettings(m_settings);
    model.generate();

    m_proxyModel = new RecursivProxyModel(parentWidget);
    m_proxyModel->setSourceModel(model.itemModel());
    m_proxyModel->setHeaderData(0, Qt::Horizontal, tr("Module"));
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_selectionModel = new QItemSelectionModel(m_proxyModel);

    //create Menu

    m_contextMenu = new QMenu(parentWidget);
    m_contextMenu->setObjectName("contextMenu");

    m_actionOpen = new QAction(QIcon::fromTheme("document-open", QIcon(":/icons/16x16/document-open.png")), tr("Open module"), m_contextMenu);
    m_actionOpen->setObjectName("actionOpen");
    connect(m_actionOpen, SIGNAL(triggered()), this, SIGNAL(open()));

    m_actionOpenInNewTab = new QAction(QIcon::fromTheme("tab-new", QIcon(":/icons/16x16/tab-new.png")), tr("Open module in new tab"), m_contextMenu);
    m_actionOpenInNewTab->setObjectName("actionOpenInNewTab");
    connect(m_actionOpenInNewTab, SIGNAL(triggered()), this, SIGNAL(openInNewTab()));

    m_actionSettings = new QAction(QIcon::fromTheme("configure", QIcon(":/icons/16x16/configure.png")), tr("Configure"), m_contextMenu);
    m_actionSettings->setObjectName("actionSettings");
    connect(m_actionSettings, SIGNAL(triggered()), this, SIGNAL(configure()));



    m_contextMenu->addAction(m_actionOpen);
    m_contextMenu->addAction(m_actionOpenInNewTab);

    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_actionSettings);
}
ModuleSelectUI::~ModuleSelectUI()
{
    m_proxyModel->deleteLater();
    m_selectionModel->deleteLater();
    m_contextMenu->deleteLater();
}

RecursivProxyModel *ModuleSelectUI::model() const
{
    return m_proxyModel;
}
QItemSelectionModel *ModuleSelectUI::selectionModel() const
{
    return m_selectionModel;
}

void ModuleSelectUI::loadModuleData(const int moduleID, Actions::OpenLinkModifiers mod)
{
    if(moduleID >= 0) {
        m_moduleID = moduleID;
        Module *m = m_moduleManager->getModule(moduleID);
        //const ModuleTools::ModuleType type = m->moduleType();
        const ModuleTools::ModuleClass cl = m->moduleClass();
        m_settings->getModuleSettings(moduleID)->stats_timesOpend++;
        switch(cl)
        {
            case ModuleTools::DictionaryModuleClass:
                m_actions->get(ModuleTools::dictScheme+ QString::number(moduleID), mod);
                break;
            case ModuleTools::WebPageClass:
                m_actions->get(ModuleTools::webPageScheme + QString::number(moduleID), mod);
                break;
            case ModuleTools::BookClass:
                m_actions->get(ModuleTools::bookScheme + QString::number(moduleID), mod);
                break;
            case ModuleTools::TreeBookClass:
                m_actions->get(ModuleTools::treeBookScheme + QString::number(moduleID) + ",0", mod);
                break;
            case ModuleTools::CommentaryClass:
            case ModuleTools::BibleModuleClass:
                m_actions->get(url(moduleID), mod);
                break;
             default:
                myWarning() << "module class not support in ModuleSelectUI";
        }
    }
}
VerseUrl ModuleSelectUI::url(const int moduleID)
{
    VerseUrlRange range;
    range.setModule(moduleID);
    range.setBook(VerseUrlRange::LoadCurrentBook);
    range.setChapter(VerseUrlRange::LoadCurrentChapter);
    range.setStartVerse(VerseUrlRange::LoadCurrentVerse);
    range.setOpenToTransformation(true);

    VerseUrl url(range);
    return url;
}

void ModuleSelectUI::selectModule(const int moduleID)
{
    if(moduleID == -1) {
        m_selectionModel->clearSelection();
        return;
    }

    if(m_moduleID == moduleID)
        return;

    m_moduleID = moduleID;
    const QModelIndexList list = m_proxyModel->match(m_moduleManager->m_moduleModel->invisibleRootItem()->index(),
                                                     Qt::UserRole + 1,
                                                     QString::number(moduleID));

    if(list.size() == 1) {
        m_selectionModel->clearSelection();
        m_selectionModel->setCurrentIndex(m_proxyModel->mapFromSource(list.first()), QItemSelectionModel::Select);
    }
}
void ModuleSelectUI::showContextMenu(const QPoint point)
{
    m_point = point;
    m_contextMenu->exec(QCursor::pos());
}

void ModuleSelectUI::showSettingsDialog(const int moduleID)
{
    if(moduleID < 0)
        return;

    QPointer<ModuleConfigDialog> mDialog = new ModuleConfigDialog;
    mDialog->setModule(m_settings->getModuleSettings(moduleID));
    connect(mDialog, SIGNAL(save(ModuleSettings)), mDialog, SLOT(close()));
    mDialog->exec();
    delete mDialog;
    m_actions->moduleChanged(moduleID);

}
void ModuleSelectUI::moduleChanged(const int moduleID)
{
    const QModelIndexList list = m_proxyModel->match(m_moduleManager->m_moduleModel->invisibleRootItem()->index(), Qt::UserRole + 1, QString::number(moduleID));

    if(list.size() == 1) {
        m_proxyModel->setData(m_proxyModel->mapFromSource(list.first()), m_settings->getModuleSettings(moduleID)->name(false), Qt::DisplayRole);
    }
}
QPoint ModuleSelectUI::point() const
{
    return m_point;
}
