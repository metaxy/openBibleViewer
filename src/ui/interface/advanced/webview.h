#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>

class WebView : public QWebView
{
Q_OBJECT
public:
    explicit WebView(QWidget *parent = 0);

signals:

public slots:

};

#endif // WEBVIEW_H
