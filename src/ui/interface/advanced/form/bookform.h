#ifndef BOOKFORM_H
#define BOOKFORM_H
#include "form.h"
#include <QWidget>

namespace Ui {
class BookForm;
}

class BookForm : public Form
{
    Q_OBJECT
    
public:
    explicit BookForm(QWidget *parent = 0);
    ~BookForm();
    
    void init();
    void restore(const QString &key);
    void save();

    Form::FormType type() const;
public slots:

    void copy();
    void selectAll();
    void print();
    void printPreview();
    void saveFile();
    QString selectedText();

    void zoomIn();
    void zoomOut();

    void activated();
private:
    Ui::BookForm *ui;
};

#endif // BOOKFORM_H
