#ifndef MODULEMODEL_H
#define MODULEMODEL_H
#include <QtGui/QStandardItemModel>
#include <QtGui/QStyle>
#include <QtGui/QApplication>
#include "src/module/module.h"
#include "src/core/settings/settings.h"
class ModuleModel
{
public:
    ModuleModel();
    ~ModuleModel();

    void clear();
    void setSettings(Settings *settings);
    void generate();
    QStandardItemModel* itemModel() const;
private:
    QStandardItemModel *m_moduleModel;
    Settings *m_settings;
    void loadModule(QStandardItem *parentItem, ModuleSettings *settings);

    QIcon m_bibleQuoteIcon;
    QIcon m_folderIcon;
    QIcon m_bibleZefaniaIcon;
};

#endif // MODULEMODEL_H
