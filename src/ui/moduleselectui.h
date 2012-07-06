#ifndef MODULESELECTUI_H
#define MODULESELECTUI_H

#include <QTreeWidgetItem>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include "src/core/basicclass.h"
#include "src/ui/recursivproxymodel.h"
class ModuleSelectUI : public QObject, public BasicClass
{
    Q_OBJECT
public:
    ModuleSelectUI(BasicClass *parent, QWidget *oarentWidget);
    ~ModuleSelectUI();
    void loadModuleData(const int moduleID, Actions::OpenLinkModifiers mod = Actions::NoModifer);    void selectModule(const int moduleID);

    void showSettingsDialog(const int moduleID);

    RecursivProxyModel *model() const;
    QItemSelectionModel *selectionModel() const;

    void moduleChanged(const int moduleID);

    QPoint point() const;

    QAction* actionOpen() const;
    QAction* actionOpenInNewTab() const;
    QAction* actionSettings() const;
public slots:
    void showContextMenu(const QPoint point);
signals:
    void open();
    void openInNewTab();
    void configure();
private:
    int m_moduleID;
    RecursivProxyModel *m_proxyModel;
    QItemSelectionModel *m_selectionModel;
    QPoint m_point;


    QMenu *m_contextMenu;
    QAction *m_actionOpen;
    QAction *m_actionOpenInNewTab;
    QAction *m_actionSettings;
    VerseUrl url(const int moduleID);

};

#endif // MODULESELECTUI_H
