#ifndef MDIFORM_H
#define MDIFORM_H

#include <QtGui/QWidget>
#include <QtGui/QCloseEvent>
#include "ui_mdiform.h"
#include "../core/history.h"
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
	void historyGo( QString );
public slots:
	void historyGetUrl(QString url);
	void backward();
	void forward();

protected:
	//   virtual void changeEvent(QEvent *e);
	//virtual void closeEvent(QCloseEvent *event);
private:
	history browserHistory;
	void setButtons();


};

#endif // MDIFORM_H
