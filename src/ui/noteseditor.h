/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef HTML_EDITOR_H
#define HTML_EDITOR_H

#include <QMainWindow>
#include "highlighter.h"
#include "src/core/settings.h"
#include "src/core/notes.h"
#include "src/module/modulemanager.h"
#include "src/core/verseselection.h"
#include "src/core/simplenotes.h"

#if QT_VERSION < 0x040500
#error You must use Qt >= 4.5.0!
#endif
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
    QString fileName;
    bool sourceDirty;
    QLabel *zoomLabel;
    QSlider *zoomSlider;
    Highlighter *highlighter;
    Ui::InsertHtmlDialog *ui_dialog;
    QDialog *insertHtmlDialog;
    SimpleNotes *m_simpleNotes;
};

#endif // HTML_EDITOR_H
