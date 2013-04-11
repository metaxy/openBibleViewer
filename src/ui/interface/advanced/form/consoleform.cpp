#include "consoleform.h"
#include "ui_consoleform.h"

ConsoleForm::ConsoleForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::ConsoleForm)
{
    ui->setupUi(this);
}

ConsoleForm::~ConsoleForm()
{
    delete ui;
}

void ConsoleForm::init()
{
    attach();
    connect(m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attach()));
    m_view->load(QUrl::fromLocalFile(":/data/html/consoleform.html"));

    connect(m_view, SIGNAL(contextMenuRequested(QContextMenuEvent*)), this, SLOT(showContextMenu(QContextMenuEvent*)));
}

void ConsoleForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrollPosition").toPoint();

    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);
}

void ConsoleForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "console");
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
}

Form::FormType ConsoleForm::type() const
{
    return Form::ConsoleForm;
}

void ConsoleForm::attach()
{
    addJS(":/data/js/tools.js");
    addJS(":/data/js/jquery-1.4.2.min.js");
    addJS(":/data/js/underscore-min.js");
    QWebFrame * frame = m_view->page()->mainFrame();
    m_view->page()->mainFrame()->addToJavaScriptWindowObject("Search", m_api->searchApi());
    addModuleApi();
    addSearchApi();
    addNotesApi();
    addActionsApi();
}
void ConsoleForm::showContextMenu(QContextMenuEvent* ev)
{
    //DEBUG_FUNC_NAME
    QScopedPointer<QMenu> contextMenu(new QMenu(this));
    QAction *actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Copy"), this);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    if(m_view->hasSelection()) {
        actionCopy->setEnabled(true);
    } else {
        actionCopy->setEnabled(false);
    }

    QAction *dbg = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/icons/16x16/edit-copy.png")), tr("Debugger"), contextMenu.data());
    connect(dbg, SIGNAL(triggered()), this, SLOT(debugger()));


    contextMenu->addAction(dbg);
    contextMenu->exec(ev->globalPos());

}
