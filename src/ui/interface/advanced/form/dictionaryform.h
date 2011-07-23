#ifndef DICTIONARYFORM_H
#define DICTIONARYFORM_H

#include <QWidget>

namespace Ui {
    class DictionaryForm;
}

#include "src/ui/interface/advanced/form/form.h"
class DictionaryForm :  public Form
{
    Q_OBJECT

public:
    explicit DictionaryForm(QWidget *parent = 0);
    ~DictionaryForm();
    void init();

    void restore(const QString &key);
    void save();

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

    void showEntry();
private:
    Ui::DictionaryForm *ui;

    Dictionary *m_dictionary;
};

#endif // DICTIONARYFORM_H
