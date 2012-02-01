#ifndef BOOKFORM_H
#define BOOKFORM_H
#include "webviewform.h"
#include <QWidget>
#include "src/module/book/book.h"
namespace Ui {
class BookForm;
}

class BookForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit BookForm(QWidget *parent = 0);
    ~BookForm();
    void init();
    void restore(const QString &key);
    void save();

    Form::FormType type() const;


    void loadModule(const int moduleID);

    void show();
public slots:
    void activated();
private:
    Ui::BookForm *ui;

    Book *m_book;
};

#endif // BOOKFORM_H
