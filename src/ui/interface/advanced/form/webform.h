#ifndef WEBFORM_H
#define WEBFORM_H

#include <QWidget>
#include <QtGui/QLineEdit>
#include "form.h"
namespace Ui {
    class WebForm;
}

class WebForm :  public Form
{
    Q_OBJECT

public:
    explicit WebForm(QWidget *parent = 0);
    ~WebForm();
    void init();
    static QUrl guessUrlFromString(const QString &string);

    void restore(const QString &key);
    void save();

    Form::FormType type() const;
private:
    Ui::WebForm *m_ui;
    QLineEdit *m_locationEdit;
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
private slots:
    void changeLocation();
    void adjustLocation();


};

#endif // WEBFORM_H
