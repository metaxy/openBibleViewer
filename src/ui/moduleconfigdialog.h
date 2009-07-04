#ifndef MODULECONFIGDIALOG_H
#define MODULECONFIGDIALOG_H

#include <QtGui/QDialog>
#include "../core/moduleconfig.h"
namespace Ui {
	class moduleConfigDialog;
}

class moduleConfigDialog : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(moduleConfigDialog)
public:
	explicit moduleConfigDialog(QWidget *parent = 0);
	virtual ~moduleConfigDialog();
	void setModule(struct moduleConfig config);

protected:
	virtual void changeEvent(QEvent *e);

private:
	Ui::moduleConfigDialog *m_ui;
	struct moduleConfig c;
signals:
	void save(struct moduleConfig config);
public slots:
	void bsave();
	void moduleTypeChanged(int id);
};

#endif // MODULECONFIGDIALOG_H
