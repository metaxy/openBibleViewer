#ifndef BIBLEMANAGER_H
#define BIBLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include "src/ui/dock/dockwidget.h"
#include "src/core/basicclass.h"
#include "src/ui/dock/bookdockwidget.h"
#include "src/ui/dock/moduledockwidget.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"
#include "src/ui/dock/quickjumpdockwidget.h"
class BibleManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit BibleManager(QObject *parent = 0);
    void init();

    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();


signals:

public slots:
    void pharseUrl(const QString &url);
private:
    AdvancedSearchResultDockWidget *m_advancedSearchResultDockWidget;
    BookDockWidget *m_bookDockWidget;
    ModuleDockWidget *m_moduleDockWidget;
    QuickJumpDockWidget * m_quickJumpDockWidget;
};

#endif // BIBLEMANAGER_H
