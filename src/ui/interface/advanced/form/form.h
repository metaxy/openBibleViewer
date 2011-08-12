#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "src/core/basicclass.h"
#include "src/ui/interface/advanced/manager/bookmarksmanager.h"
#include "src/ui/interface/advanced/manager/biblemanager.h"
#include "src/ui/interface/advanced/manager/notesmanager.h"
#include "src/api/api.h"
#include "src/core/dbghelper.h"
#include <QtGui/QMdiSubWindow>
class Form : public QWidget, public BasicClass
{
    Q_OBJECT
public:
    enum FormType {
        BibleForm,
        DictionaryForm,
        WebForm
    };

    explicit Form(QWidget *parent = 0);
    virtual void init() = 0;
    virtual void setID(const int id);
    virtual int id();

    void setApi(Api *api);
    void setBibleManager(BibleManager *bibleManager);
    void setNotesManager(NotesManager *notesManager);
    void setBookmarksManager(BookmarksManager *bookmarksManager);
    void setParentSubWindow(QMdiSubWindow *window);

    int *currentWindowID;

    virtual void restore(const QString &key) = 0;
    virtual void save() = 0;

    virtual FormType type() const = 0;
signals:

public slots:

    virtual void copy() = 0;
    virtual void selectAll() = 0;
    virtual void print() = 0;
    virtual void printPreview() = 0;
    virtual void saveFile() = 0;
    virtual QString selectedText() = 0;

    virtual void zoomIn() = 0;
    virtual void zoomOut() = 0;

    virtual void activated() = 0;
protected:

    bool active();
    int m_id;
    Api *m_api;//not in out control
    BibleManager *m_bibleManager;
    NotesManager *m_notesManager;
    BookmarksManager *m_bookmarksManager;
    QMdiSubWindow *m_parentSubWindow;

};

#endif // FORM_H
