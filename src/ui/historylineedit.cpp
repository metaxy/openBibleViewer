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
    }
    if(m_completer != nullptr) {
        delete m_completer;
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
    myDebug() << s;
    QStringList list = m_model->stringList();
    list << s;
    m_model->setStringList(list);
}
QStringList HistoryLineEdit::history() const
{
    return m_model->stringList();
}
