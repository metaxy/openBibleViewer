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
    Form::FormType type() const;
    Dictionary* dictionary() const;
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
    void showEntry(const QString &key, int moduleID=-1);
private slots:
    void forwardShowHtml(const QString &html);
    void forwardShowEntry(const QString &key, int moduleID);

    void get(QUrl url);
private:
    Ui::DictionaryForm *ui;
    void loadDictionary(int moduleID);
    void testDictionary(int moduleID = -1);
    Dictionary *m_dictionary;
    void showHtml(const QString &html);

    QString m_key;
};

#endif // DICTIONARYFORM_H
