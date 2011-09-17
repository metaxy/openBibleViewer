#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H
#include "src/core/basicclass.h"
#include "src/ui/dock/dictionarydockwidget.h"
#include "src/ui/interface/advanced/manager/windowmanager.h"

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

    void open(const QString &key, ModuleSettings::DefaultModule defaultModule);

    void setWindowManager(WindowManager *windowManager);
private:
    QWidget *m_p;
    DictionaryDockWidget *m_dictionaryDock;
    WindowManager *m_windowManager;
};

#endif // DICTIONARYMANAGER_H
