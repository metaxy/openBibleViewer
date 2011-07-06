#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "src/core/basicclass.h"

class Form : public QWidget, public BasicClass
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = 0);

signals:

public slots:

    virtual void copy() = 0;
    virtual void selectAll() = 0;
    virtual void print() = 0;
    virtual void printPreview() = 0;
    virtual void saveFile() = 0;
    virtual QString selectedText() = 0;

    virtual void zoomIn() = 0;
    virtual void zoomOut() = 0;

};

#endif // FORM_H
