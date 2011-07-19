#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H
#include "src/core/basicclass.h"
#include "src/ui/dock/dictionarydockwidget.h"
#include <QObject>

class DictionaryManager : public QObject, public BasicClass
{
    Q_OBJECT
public:
    DictionaryManager(QObject *parent = 0);
    void setWidget(QWidget *p);
    void createDocks();
    QHash<DockWidget *, Qt::DockWidgetArea> docks();
    DictionaryDockWidget *dictionaryDockWidget();

    void pharseUrl(QString url);
private:
    QWidget *m_p;
    DictionaryDockWidget *m_dictionaryDock;
};

#endif // DICTIONARYMANAGER_H
