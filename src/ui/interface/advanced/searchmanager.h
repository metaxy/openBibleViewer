#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H
#include <QtCore/QHash>
#include "windowmanager.h"
#include "src/ui/dock/dockwidget.h"
#include "src/core/basicclass.h"
#include "src/ui/dock/advancedsearchresultdockwidget.h"

class SearchManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    explicit SearchManager(QObject *parent = 0);
    void setWidget(QWidget *p);
    void setWindowManager(WindowManager *windowManager);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
signals:

public slots:

    void search(SearchQuery query);
    void search();
    void showSearchDialog();
    void nextVerse();
    void previousVerse();
private:
    QWidget *m_p;
    WindowManager *m_windowManager;
    AdvancedSearchResultDockWidget *m_advancedSearchResultDockWidget;
};

#endif // SEARCHMANAGER_H
