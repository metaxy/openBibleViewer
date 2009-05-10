#ifndef MDIFORM_H
#define MDIFORM_H

#include <QtGui/QWidget>
#include "ui_mdiform.h"

namespace Ui {
    class mdiForm;
}

class mdiForm : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(mdiForm)
public:
	explicit mdiForm(QWidget *parent = 0);
	virtual ~mdiForm();
	Ui::mdiForm *m_ui;
signals:
        void onClose();

protected:
        virtual void changeEvent(QEvent *e);
        virtual void closeEvent(QCloseEvent *event);
private:


};

#endif // MDIFORM_H
