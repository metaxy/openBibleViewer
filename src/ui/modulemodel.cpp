#include "modulemodel.h"

ModuleModel::ModuleModel()
{
    m_moduleModel = new QStandardItemModel;
}
ModuleModel::~ModuleModel()
{
    //delete m_moduleModel;
}
void ModuleModel::clear()
{
    m_moduleModel->clear();
}

void ModuleModel::setSettings(Settings *settings)
{
    m_settings = settings;
}

void ModuleModel::generate()
{
    DEBUG_FUNC_NAME
    QStandardItem *parentItem = m_moduleModel->invisibleRootItem();

    QIcon bibleQuoteIcon = QIcon::fromTheme("text-x-generic", QIcon(":/icons/16x16/text-x-generic.png"));
    QStyle *style = QApplication::style();
    QIcon folderIcon;
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);

    QIcon bibleZefaniaIcon =  QIcon::fromTheme("text-xml", QIcon(":/icons/16x16/text-xml.png"));


    ModuleSettings rootModuleSettings = m_settings->getModuleSettings(-1);//it the invisble root item

    foreach(ModuleSettings * s, rootModuleSettings.children()) {
        loadModule(parentItem, s);
    }
}

QStandardItemModel* ModuleModel::itemModel() const
{
    return m_moduleModel;
}
void ModuleModel::loadModule(QStandardItem *parentItem, ModuleSettings *settings)
{
    QStandardItem *item;
    if(settings->moduleType == OBVCore::BibleQuoteModule || settings->moduleType == OBVCore::ZefaniaBibleModule || settings->moduleType == OBVCore::TheWordBibleModule) {
        item = new QStandardItem;
        item->setText(settings->moduleName);
        item->setData(QString::number(settings->moduleID));
        item->setToolTip(Module::moduleTypeName(settings->moduleType) + " - " + settings->modulePath + " (" + QString::number(settings->moduleID) + ")");

        //todo: icons
        //item->setIcon(bibleQuoteIcon);
        parentItem->appendRow(item);
    } else if(settings->moduleType == OBVCore::ZefaniaLexModule || settings->moduleType == OBVCore::BibleQuoteDictModule) {
    }

    //recursive
    foreach(ModuleSettings * s, settings->children()) {
        loadModule(item, s);
    }

}
