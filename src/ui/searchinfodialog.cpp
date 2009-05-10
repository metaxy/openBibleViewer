/***************************************************************************
openBibleViewer - Free Bibel Viewer
Copyright (C) 2009 Paul Walger
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option)
any later version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
#include "searchinfodialog.h"
#include "ui_searchinfodialog.h"
#include <QPainter>
#include <QtDebug>
#include <QGraphicsPixmapItem>
#include <math.h>
searchInfoDialog::searchInfoDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::searchInfoDialog)
{
    m_ui->setupUi(this);
    connect( m_ui->pushButton_close, SIGNAL( clicked() ), this, SLOT( close() ) );
}

searchInfoDialog::~searchInfoDialog()
{
    delete m_ui;
}
void searchInfoDialog::setInfo(struct stelle st,QStringList bookNames,QString searchText)
{
	m_ui->label_searchText->setText( tr("Search String : '%1'").arg(searchText));
	m_ui->label_foundItems->setText(tr("Found Verses : %1").arg(QString::number(st.verse.size())));
	QMap<int, int> booksMap;
	for(int i =0; i < st.book.size();i++)
	{
		booksMap[st.book.at(i)]++;
	}
	qDebug() << "searchInfoDialog::setInfo booksMap="<<booksMap;
	//stat
	int x = 10;
	int mWidth= 290;
	int padding = 5;
	int height = 20;
	int countBooks=0;
	int maxVerse=0;
	QMapIterator<int, int> i2(booksMap);
	while (i2.hasNext())
	{
		i2.next();
		if(i2.value() > maxVerse)
		{
			maxVerse = i2.value();
		}
		countBooks++;
	}
	QGraphicsScene *scene = new QGraphicsScene(this);
	m_ui->graphicsView->setScene(scene);
	QPixmap pm(300,countBooks*(height+(height/4))+30);
	pm.fill();

	QPainter p(&pm);

	QMapIterator<int, int> i(booksMap);
	int c = 0;
	while (i.hasNext())
	{
		c++;
		int color;
		if(c % 2 == 0)
		{
			color = 206;
		}
		else
		{
			color = 108;
		}
		i.next();
		QPen pen(Qt::black,1);
		p.setPen(pen);
		int s = d2i((255/maxVerse)*i.value());
		QLinearGradient linearGrad(QPointF(padding, x), QPointF(padding, x+(height/2)) );
		linearGrad.setColorAt(0,  Qt::white );
		linearGrad.setColorAt(1, QColor::fromHsv(color,s,255));
		QBrush brush(linearGrad);
		p.setBrush(brush);



		int width = d2i((i.value() * 100)/st.book.size()) * ( mWidth / ((maxVerse*100)/st.book.size())) ;
		QRectF rectangle(padding,x,width,height);

		p.drawRect(rectangle);
		double prozent = mRound((i.value() * 100)/st.book.size(),2);
		p.drawText(padding+(padding/2),x+height-4,bookNames.at(i.key()) + " - " + QString::number(i.value()) + "(" + QString::number(prozent) + "%)");
		x += height+(height/4);
	}
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem(pm);
	item = scene->addPixmap(pm);
	m_ui->graphicsView->show();
	return;
}
double searchInfoDialog::mRound(double Zahl, int Stellen)
{
	return floor(Zahl * pow( 10, Stellen) + 0.5) * pow(10, -Stellen);
}
int searchInfoDialog::d2i(double d)
{
	return d<0?d-.5:d+.5;
}
void searchInfoDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
