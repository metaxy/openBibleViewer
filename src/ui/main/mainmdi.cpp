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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../mdiform.h"

#include <QtCore/QtDebug>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSizePolicy>
#include <QtGui/QMdiSubWindow>
void MainWindow::newMdiChild()
{
    //qDebug() << "MainWindow::newMdiChild()";
    int windowsCount = usableWindowList().size();
    QMdiSubWindow *firstSubWindow = new QMdiSubWindow();
    if (windowsCount == 1) {
        firstSubWindow = usableWindowList().at(0);
    }
    tcache.newWindow();
    QWidget *widget = new QWidget(ui->mdiArea);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    mdiForm *mForm = new mdiForm(this);
    layout->addWidget(mForm);
    widget->setLayout(layout);
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(widget);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    subWindow->setWindowTitle("");
    //qDebug() << "MainWindow::newMdiChild() first windowsCount = " << windowsCount;
    if (windowsCount == 0) {
        //qDebug() << "First Window";
        subWindow->showMaximized();
    } else if (windowsCount == 1) {
        firstSubWindow->resize(600, 600);
        firstSubWindow->showNormal();
        subWindow->resize(600, 600);
        subWindow->show();
    } else {
        subWindow->resize(600, 600);
        subWindow->show();
    }

    connect(mForm->m_ui->comboBox_books, SIGNAL(activated(int)), this, SLOT(readBook(int)));
    connect(mForm->m_ui->comboBox_chapters, SIGNAL(activated(int)), this, SLOT(readChapter(int)));
    connect(mForm->m_ui->textBrowser, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(textBrowserContextMenu(QPoint)));
    connect(mForm->m_ui->textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(pharseUrl(QUrl)));
    connect(mForm, SIGNAL(historyGo(QString)), this, SLOT(pharseUrl(QString)));
    connect(this, SIGNAL(historySetUrl(QString)), mForm, SLOT(historyGetUrl(QString)));
    connect(subWindow, SIGNAL(destroyed(QObject*)), this, SLOT(closeWindow()));

    if (windowsCount >= 1) {
        if (set.autoLayout == 1) {
            myTileVertical();
        } else if (set.autoLayout == 2) {
            myTileHorizontal();
        } else if (set.autoLayout == 3) {
            myCascade();
        }
    }

    internalWindows << subWindow;
    enableReload = true;
    //add a this new subwindow in the list
}
QMdiSubWindow *MainWindow::activeMdiChild()
{
    QList<QMdiSubWindow*> list = usableWindowList();
    if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow()) {
        for (int i = 0; i < list.size(); i++) {
            if (list.at(i) == activeSubWindow) {
                lastActiveWindow = i;
            }
        }
        return activeSubWindow;
    } else if (lastActiveWindow < list.size()) {
        ui->mdiArea->setActiveSubWindow(usableWindowList().at(lastActiveWindow));
        if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
            return activeSubWindow;
    } else if (usableWindowList().count() > 0) {
        ui->mdiArea->setActiveSubWindow(usableWindowList().at(0));
        if (QMdiSubWindow *activeSubWindow = ui->mdiArea->activeSubWindow())
            return activeSubWindow;
    }
    return 0;
}
void MainWindow::myTileVertical()
{
    if (!enableReload ||  !usableWindowList().count()) {
        return;
    }

    QList<QMdiSubWindow*> windows = usableWindowList();
    if (ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if (ui->mdiArea->activeSubWindow()->size() != this->size()) {
            ui->mdiArea->activeSubWindow()->resize(this->size());
        }
    } else if (windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {
        enableReload = false;
        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();
        ui->mdiArea->tileSubWindows();
        if (active) active->setFocus();
        enableReload = true;
    }
}

void MainWindow::myTileHorizontal()
{
    if (!enableReload || !usableWindowList().count()) {
        return;
    }
    QList<QMdiSubWindow*> windows = usableWindowList();

    if (ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if (ui->mdiArea->activeSubWindow()->size() != ui->mdiArea->size()) {
            ui->mdiArea->activeSubWindow()->resize(ui->mdiArea->size());
        }
    } else if (windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {
        enableReload = false;

        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();

        const int heightForEach = ui->mdiArea->height() / windows.count();
        unsigned int y = 0;
        foreach(QMdiSubWindow *window, windows) {
            window->showNormal();

            const int preferredHeight = window->minimumHeight() + window->baseSize().height();
            const int actHeight = qMax(heightForEach, preferredHeight);

            window->setGeometry(0, y, ui->mdiArea->width(), actHeight);
            y += actHeight;
        }
        active->setFocus();
        enableReload = true;
    }
}

void MainWindow::myCascade()
{
    if (!enableReload || !usableWindowList().count()) {
        return;
    }

    QList<QMdiSubWindow*> windows = usableWindowList();

    if (ui->mdiArea->activeSubWindow() && ui->mdiArea->activeSubWindow()->isMaximized()) {
        if (ui->mdiArea->activeSubWindow()->size() != this->size()) {
            ui->mdiArea->activeSubWindow()->resize(this->size());
        }
    } else if (windows.count() == 1) {
        windows.at(0)->showMaximized();
    } else {

        QMdiSubWindow* active = ui->mdiArea->activeSubWindow();
        enableReload = false;
        const unsigned int offsetX = 40;
        const unsigned int offsetY = 40;
        const unsigned int windowWidth =  ui->mdiArea->width() - (windows.count() - 1) * offsetX;
        const unsigned int windowHeight = ui->mdiArea->height() - (windows.count() - 1) * offsetY;
        unsigned int x = 0;
        unsigned int y = 0;

        foreach(QMdiSubWindow* window, windows) {
            if (window == active)//leave out the active window which should be the top window
                continue;
            window->raise(); //make it the on-top-of-window-stack window to make sure they're in the right order
            window->setGeometry(x, y, windowWidth, windowHeight);
            x += offsetX;
            y += offsetY;
        }
        active->setGeometry(x, y, windowWidth, windowHeight);
        active->raise();
        active->activateWindow();
        enableReload = true;
    }
}
QList<QMdiSubWindow*> MainWindow::usableWindowList()
{
    //only if !ChildAdded-Event is triggered
    QList<QMdiSubWindow*> ret;
    foreach(QMdiSubWindow* w, ui->mdiArea->subWindowList()) {
        if (w->isMinimized() /*|| w->isHidden()*/) //not usable for us
            continue;
        ret.append(w);
    }
    return ret;
}
int MainWindow::currentTabID()
{
    //qDebug()  << "MainWindow::currentTabID()";
    for (int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
        if (ui->mdiArea->activeSubWindow() ==  ui->mdiArea->subWindowList().at(i)) {
            //qDebug()  << "MainWindow::currentTabID() id = " << i;
            return i;
        }
    }
    return -1;
}
int MainWindow::tabIDof(QMdiSubWindow* window)
{
    //qDebug()  << "MainWindow::tabIDof()";
    for (int i = 0; i < ui->mdiArea->subWindowList().count(); i++) {
        if (window ==  ui->mdiArea->subWindowList().at(i)) {
            //qDebug()  << "MainWindow::tabIDof() id = " << i;
            return i;
        }
    }

    return -1;
}
int MainWindow::closeWindow()
{
    qDebug() << "MainWindow::closeWindow()";
    if (!enableReload) {
        qDebug() << "MainWindow::closeWindow() reload is not enabled";
        return 1;
    }
    //if one in the internal subwindow list list is missing that window was closed
    if (ui->mdiArea->subWindowList().isEmpty()) {
        qDebug() << "MainWindow::closeWindow() subWIndowList is empty";
        setBooks(QStringList());
        setChapters(QStringList());
        tcache.clearAll();
        return 1;
    }
    if (internalWindows.isEmpty()) {
        qDebug() << "MainWindow::closeWindow() internaL is empty";
        setBooks(QStringList());
        setChapters(QStringList());
        tcache.clearAll();
        return 1;
    }
    qDebug() << "MainWindow::closeWindow() closing";
    for (int i = 0; i < internalWindows.size(); i++) {
        if (ui->mdiArea->subWindowList().lastIndexOf(internalWindows.at(i)) == -1) {
            qDebug() << "MainWindow::closeWindow() found closed Window id = " << i;
            tcache.removeTab(i);
            internalWindows.removeAt(i);
            break;
        }
    }
    if (ui->mdiArea->subWindowList().isEmpty()) { //last window closed
        qDebug() << "MainWindow::closeWindow() last closed";
        setBooks(QStringList());
        setChapters(QStringList());
        tcache.clearAll();
        return 1;
    }
    reloadWindow(ui->mdiArea->currentSubWindow());
    return 0;
}
int MainWindow::reloadWindow(QMdiSubWindow * window)
{
    if (!enableReload) {
        //qDebug() << "MainWindow::reloadWindow() reload is not enabled";
        return 1;
    }
    int id = tabIDof(window);
    //qDebug() << "MainWindow::reloadWindow() bibletype = " << tcache.getBibleType() << ", id = " << id;
    if (id == -1) {
        //qDebug() << "MainWindow::reloadWindow() id == -1";
        return 1;
    }
    if (ui->mdiArea->subWindowList().count() <= 0) {
        //qDebug() << "MainWindow::reloadWindow() count <= 0";
        return 1;
    }

    qDebug() << "MainWindow::reloadWindow() setCurrentTab id = " << id;
    tcache.setCurrentTabId(id);
    if (tcache.getBibleType() == 0) { //probaly no bible loaded in this window
        qDebug() << "MainWindow::reloadWindow() tcache.getBibleType() == 0";
        ui->listWidget_chapters->clear();
        ui->listWidget_books->clear();
        m_bible.currentBibleID = -2;
    } else {
        if (m_bible.currentBibleID == tcache.getBible().currentBibleID)
            return 1;
        qDebug() << "MainWindow::reloadWindow() get bible";
        m_bible = tcache.getBible();
        setTitle(tcache.getBibleName());
        qDebug() << "MainWindow::reloadWindow() m_bible.chapterNames.size() = " << m_bible.chapterNames.size() << ", m_bible.currentChapterID = " << m_bible.currentChapterID;
        setChapters(m_bible.chapterNames);
        setCurrentChapter(m_bible.currentChapterID);

        QStringList tbookFullName = tcache.getBooks();//show all books
        setBooks(tbookFullName);
        setCurrentBook(tcache.getCurrentBook());
        //currentBibleID = m_bible.currentBibleID;
    }

    return 0;
}
