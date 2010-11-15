/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2010 Paul Walger
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


#ifndef HTML_EDITOR_H
#define HTML_EDITOR_H

#include <QMainWindow>
#include "highlighter.h"
#include "src/core/settings/settings.h"
#include "src/core/notes.h"
#include "src/module/modulemanager.h"
#include "src/core/verseselection.h"
#include "src/core/simplenotes.h"

namespace Ui
{
class NotesEditor;
class InsertHtmlDialog;
}


class QLabel;
class QSlider;
class QUrl;

class NotesEditor : public QMainWindow, public BasicClass
{
    Q_OBJECT

signals:
    void get(QUrl url);
public:
    NotesEditor(QWidget *parent = 0);
    ~NotesEditor();
    void init();

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    void setupActions();
    void setupToolBar();
    void setupMenu();
    void execCommand(const QString&);
    void execCommand(const QString &cmd, const QString &arg);
    bool queryCommandState(const QString&);

private slots:
    void fileNew();
    bool fileSave();
    bool fileSaveAs();
    void editSelectAll();
    void styleParagraph();
    void styleHeading1();
    void styleHeading2();
    void styleHeading3();
    void styleHeading4();
    void styleHeading5();
    void styleHeading6();
    void stylePreformatted();
    void styleAddress();
    void formatStrikeThrough();
    void formatAlignLeft();
    void formatAlignCenter();
    void formatAlignRight();
    void formatAlignJustify();
    void formatIncreaseIndent();
    void formatDecreaseIndent();
    void formatNumberedList();
    void formatBulletedList();
    void formatFontName();
    void formatFontSize();
    void formatTextColor();
    void formatBackgroundColor();
    void insertImage();
    void createLink();
    void insertHtml();
    void zoomOut();
    void zoomIn();
    void adjustActions();
    void adjustSource();
    void changeTab(int);
    void changeZoom(int);
    void createLink(QString link);
    void pharseUrl(QUrl url);

private:
    Ui::NotesEditor *ui;
    bool m_sourceDirty;
    QLabel *m_zoomLabel;
    QSlider *m_zoomSlider;
    Highlighter *m_highlighter;
    Ui::InsertHtmlDialog *ui_dialog;
    QDialog *m_insertHtmlDialog;
    SimpleNotes *m_simpleNotes;
};

#endif // HTML_EDITOR_H
