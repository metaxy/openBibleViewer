#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QtDebug>
void MainWindow::loadStrongs()
{
	currentStrongID = "";
	currentStrongModule = -1;
	return;
}
QString MainWindow::loadStrong(QString strongID)
{
	qDebug() << " MainWindow::loadStrong";
	zefStrong.setSettings(set,moduleConfig());
	if(currentStrongModule == -1)
	{
		int id = -1;
		//load strong module
		for(int i = 0;i < set.module.size(); ++i)
		{
			if(set.module.at(i).moduleType == "3")
			{
				id = i;
				break;
			}
		}
		if(id >= 0)
			zefStrong.loadDataBase(set.module.at(id).modulePath);
	}
	return zefStrong.getStrong(strongID);
}
void MainWindow::showStrong(QString strongID)
{
	ui->textBrowser_strong->setText(loadStrong(strongID));
}
void MainWindow::strongSearch()
{
	qDebug() << "MainWindow::strongSearch()";
	QString search = ui->lineEdit_strong->text();
	showStrong(search);
}
