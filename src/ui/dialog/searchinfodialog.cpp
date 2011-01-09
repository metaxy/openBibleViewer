/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger
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
#include <QtGui/QPainter>
#include <QtGui/QGraphicsPixmapItem>
#include "src/core/dbghelper.h"
#include <math.h>
SearchInfoDialog::SearchInfoDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SearchInfoDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    m_textShown = false;
}

SearchInfoDialog::~SearchInfoDialog()
{
    delete m_ui;
}
void SearchInfoDialog::setInfo(SearchResult result, Versification *v11n, QString searchText, QStringList textList)
{
    int size = result.hits().size();
    m_ui->label_searchText->setText(tr("Search string : '%1'").arg(searchText));
    m_ui->label_foundItems->setText(tr("Found verses : %1").arg(QString::number(size)));
    QMap<int, int> booksMap;
    QList<SearchHit> hitList = result.hits(SearchHit::BibleHit);
    foreach(const SearchHit & hit, hitList) {
        booksMap[hit.value(SearchHit::BookID).toInt()]++;
    }


    //stat
    const int mWidth = 290;
    const int padding = 5;
    const int height = 20;
    int x = 10;
    int countBooks = 0;
    int maxVerse = 0;
    QMapIterator<int, int> i2(booksMap);
    while(i2.hasNext()) {
        i2.next();
        if(i2.value() > maxVerse) {
            maxVerse = i2.value();
        }
        countBooks++;
    }
    QGraphicsScene *scene = new QGraphicsScene(this);
    m_ui->graphicsView->setScene(scene);
    QPixmap pm(300, countBooks*((5 / 4)*height) + 30);
    pm.fill();
    QPainter p(&pm);

    QMapIterator<int, int> i(booksMap);
    int c = 0;
    while(i.hasNext()) {
        c++;
        int color;
        if(c % 2 == 0) {
            color = 223;
        } else {
            color = 0;
        }
        i.next();
        QPen pen(Qt::black, 1);
        p.setPen(pen);
        int s = d2i((double)(255 / (double) maxVerse) * (double)i.value());
        QLinearGradient linearGrad(QPointF(padding, x), QPointF(padding, x + (height / 2)));
        linearGrad.setColorAt(0,  Qt::white);
        linearGrad.setColorAt(1, QColor::fromHsv(color, s, 255));
        QBrush brush(linearGrad);
        p.setBrush(brush);

        double width = ((double)(i.value() * 100) / size) * (mWidth / ((maxVerse * 100) / size)) ;
        QRectF rectangle(padding, x, width, height);

        p.drawRect(rectangle);
        double prozent = mRound((i.value() * 100) / size, 2);
        p.drawText((3 / 2)*padding, x + height - 4, v11n->bookName(i.key()) + " - " + QString::number(i.value()) + "(" + QString::number(prozent) + "%)");
        x += (5 / 4) * height;
    }
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pm);
    item = scene->addPixmap(pm);
    m_ui->graphicsView->show();
    m_textList = textList;

    return;
}
void SearchInfoDialog::tabChanged(int index)
{
    //DEBUG_FUNC_NAME
    //myDebug() << "index = " << index;
    if(index == 1 && m_textShown == false) {  //tab_2
        m_ui->textBrowser_list->setHtml(m_textList.join("<br /><hr>"));
        m_textShown = true;
    }
}

double SearchInfoDialog::mRound(double number, int stellen)
{
    return floor(number * pow(10.0, stellen) + 0.5) * pow(10.0, -stellen);
}
int SearchInfoDialog::d2i(double d)
{
    return d < 0 ? d - .5 : d + .5;
}
void SearchInfoDialog::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
