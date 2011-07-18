#ifndef DICTIONARYFORM_H
#define DICTIONARYFORM_H

#include <QWidget>

namespace Ui {
    class DictionaryForm;
}

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
private:
    Ui::DictionaryForm *ui;
};

#endif // DICTIONARYFORM_H
