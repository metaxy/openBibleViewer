#ifndef WEBVIEWFORM_H
#define WEBVIEWFORM_H
#include "form.h"
#include "src/ui/webview.h"
class WebViewForm : public Form
{
    Q_OBJECT
public:
    explicit WebViewForm(QWidget *parent = 0);

    WebView *m_view;
signals:
    
public slots:
    virtual void copy();
    virtual void selectAll();
    virtual void print();
    virtual void printPreview();
    virtual void saveFile();
    virtual QString selectedText();

    virtual void zoomIn();
    virtual void zoomOut();

protected:

};

#endif // WEBVIEWFORM_H
