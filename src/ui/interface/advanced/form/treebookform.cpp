#include "treebookform.h"
#include "ui_treebookform.h"
#include "src/module/response/urlresponse.h"
#include "src/module/response/stringresponse.h"
TreeBookForm::TreeBookForm(QWidget *parent) :
    WebViewForm(parent),
    ui(new Ui::TreeBookForm),
    m_book(NULL)
{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(m_view);
    connect(ui->treeView, SIGNAL(activated(QModelIndex)), this, SLOT(loadChapter(QModelIndex)));
}

TreeBookForm::~TreeBookForm()
{
    if(m_book != NULL) {
        delete m_book;
        m_book = NULL;
    }
    delete ui;
}

void TreeBookForm::init()
{

}

void TreeBookForm::restore(const QString &key)
{
    const QString a = m_settings->session.id() + "/windows/" + key + "/";
    const qreal zoom = m_settings->session.file()->value(a + "zoom").toReal();
    const QPoint scroll = m_settings->session.file()->value(a + "scrool").toPoint();
    /*const QString uid = m_settings->session.file()->value(a + "uid").toString();
    int moduleID = -1;
    foreach(Module * m, m_moduleManager->m_moduleMap->data) {
        if(m->moduleUID() == uid) {
            moduleID = m->moduleID();
            break;
        }
    }
    if(moduleID != -1) {
        loadModule(moduleID);
        show();
    }*/

    m_view->page()->mainFrame()->setScrollPosition(scroll);
    m_view->setZoomFactor(zoom);
}

void TreeBookForm::save()
{
    const QString a = m_settings->session.id() + "/windows/" + QString::number(m_id) + "/";
    m_settings->session.file()->setValue(a + "type", "book");
    m_settings->session.file()->setValue(a + "scrollPosition", m_view->page()->mainFrame()->scrollPosition());
    m_settings->session.file()->setValue(a + "zoom",  m_view->zoomFactor());
    /*if(m_book != NULL) {
        m_settings->session.file()->setValue(a + "uid", m_moduleManager->getModule(m_book->moduleID())->moduleUID());
    }*/
}

Form::FormType TreeBookForm::type() const
{
    return Form::TreeBookForm;
}

void TreeBookForm::activated()
{
    if(m_book == NULL)
        return;
    m_actions->setTitle(m_book->moduleTitle());
    m_actions->setCurrentModule(m_book->moduleID());
}
void TreeBookForm::loadModule(const int moduleID)
{
    if(m_book == NULL) {
        m_book = new TreeBook();
        m_moduleManager->initSimpleModule(m_book);
    } else {
        m_book->clearData();
    }
    ModuleTools::ModuleType type = m_moduleManager->getModule(moduleID)->moduleType();
    m_book->setModuleType(type);
    m_book->setModuleID(moduleID);
    m_book->loadModuleData(moduleID);

    m_actions->setTitle(m_book->moduleTitle());
    m_actions->setCurrentModule(m_book->moduleID());

    QStandardItemModel *model = new QStandardItemModel(ui->treeView);
    QStandardItem *parentItem = model->invisibleRootItem();
    BookTree *tree = m_book->bookTree();
    for (int i = 0; i < tree->children.size(); ++i) {
        QStandardItem *item = new QStandardItem(tree->children.at(i)->title);
        item->setData(tree->children.at(i)->id);
        parentItem->appendRow(item);
    }
    ui->treeView->setModel(model);

}

void TreeBookForm::showChapter(const int moduleID, const int chapterID)
{
    if(!m_book) {
        myWarning() << "m_com is null";
        return;
    }
    m_book->setModuleID(moduleID);
    Response *res = m_book->readChapter(chapterID);

    if(res != NULL && res->isValid()) {
        if(res->type() == Response::UrlReponse) {
            UrlResponse *u = (UrlResponse*) res;
            m_view->load(QUrl(u->url()));
        } else if(res->type() == Response::StringReponse) {
            StringResponse *u = (StringResponse*) res;
            m_view->setHtml(u->data());
        } else if(res->type() == Response::HtmlResponse) {
            StringResponse *u = (StringResponse*) res;
            m_view->setHtml(u->data());
        }
    }

}
void TreeBookForm::loadChapter(QModelIndex index)
{
    DEBUG_FUNC_NAME
    const int chapterID = index.data(Qt::UserRole + 1).toInt();
    showChapter(m_book->moduleID(), chapterID);
}

