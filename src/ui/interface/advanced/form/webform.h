#ifndef WEBFORM_H
#define WEBFORM_H

#include <QWidget>
#include <QtGui/QLineEdit>
#include "form.h"
#include "src/module/webpage.h"
namespace Ui {
    class WebForm;
}

class WebForm : public Form
{
    Q_OBJECT
    Q_DISABLE_COPY(WebForm)
public:
    explicit WebForm(QWidget *parent = 0);
    ~WebForm();
    void init();
    static QUrl guessUrlFromString(const QString &string);

    void restore(const QString &key);
    void save();

    void pharseUrl(QString url);

    Form::FormType type() const;

    void openModule(const int moduleID);

private:
    Ui::WebForm *m_ui;
    QLineEdit *m_locationEdit;

    WebPage *m_page;
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

    void loadStarted();


};

#endif // WEBFORM_H
