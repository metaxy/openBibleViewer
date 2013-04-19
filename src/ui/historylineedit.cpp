/***************************************************************************
openBibleViewer - Bible Study Tool
Copyright (C) 2009-2012 Paul Walger <metaxy@walger.name>
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
#include "historylineedit.h"
#include "src/core/dbghelper.h"
HistoryLineEdit::HistoryLineEdit(QWidget* parent): QLineEdit(parent), m_completer(nullptr), m_model(nullptr)
{
    setHistory(QStringList());
}

void HistoryLineEdit::keyPressEvent(QKeyEvent* e)
{
    QLineEdit::keyPressEvent(e);
}
void HistoryLineEdit::setHistory(const QStringList& hist)
{
    myDebug() << hist;
    if(m_model != nullptr) {
        delete m_model;
        m_model = nullptr;
    }
    if(m_completer != nullptr) {
        delete m_completer;
        m_completer = nullptr;
    }
    m_model = new QStringListModel(hist);
    m_completer = new QCompleter(m_model, this);
    setCompleter(m_completer);
    //connect(m_completer, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
    connect(this, SIGNAL(editingFinished()), this, SLOT(addToHistory()));
    
}
void HistoryLineEdit::addToHistory()
{
    addToHistory(text());
}

void HistoryLineEdit::addToHistory(const QString& s)
{
    QStringList list = m_model->stringList();
    if(!list.contains(s)) {
        list << s;
        m_model->setStringList(list);
    }
}
QStringList HistoryLineEdit::history() const
{
    return m_model->stringList();
}
