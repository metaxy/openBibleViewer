#ifndef TREEBOOKFORM_H
#define TREEBOOKFORM_H
#include "webviewform.h"
#include "src/module/book/treebook.h"
#include <QWidget>

namespace Ui {
class TreeBookForm;
}

class TreeBookForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit TreeBookForm(QWidget *parent = 0);
    ~TreeBookForm();

    void init();
    void restore(const QString &key);
    void save();

    Form::FormType type() const;


    void loadModule(const int moduleID);

    void showChapter(const int moduleID, const int chapterID);
public slots:
    void activated();
    void loadChapter(QModelIndex index);
private:
    Ui::TreeBookForm *ui;

    TreeBook *m_book;
};

#endif // TREEBOOKFORM_H
