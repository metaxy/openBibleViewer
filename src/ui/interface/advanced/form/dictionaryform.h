/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2011 Paul Walger <metaxy@walger.name>
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
#ifndef DICTIONARYFORM_H
#define DICTIONARYFORM_H

#include <QWidget>

namespace Ui {
    class DictionaryForm;
}

#include "src/ui/interface/advanced/form/form.h"
class DictionaryForm :  public Form
{
    Q_OBJECT

public:
    explicit DictionaryForm(QWidget *parent = 0);
    ~DictionaryForm();
    void init();

    void restore(const QString &key);
    void save();
    Form::FormType type() const;
    Dictionary* dictionary() const;
public slots:
    void copy();
    void selectAll();
    void print();
    void printPreview();
    void saveFile();
    QString selectedText();

    void zoomIn();
    void zoomOut();
    void activated();

    void showEntry();
    void showEntry(const QString &key, int moduleID=-1);
private slots:
    void forwardShowHtml(const QString &html);
    void forwardShowEntry(const QString &key, int moduleID);

    void get(QUrl url);
private:
    Ui::DictionaryForm *ui;
    void loadDictionary(int moduleID);
    void testDictionary(int moduleID = -1);
    Dictionary *m_dictionary;
    void showHtml(const QString &html);

    QString m_key;
};

#endif // DICTIONARYFORM_H
