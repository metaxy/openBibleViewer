
#ifndef HISTORYLINEEDIT_H
#define HISTORYLINEEDIT_H

#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>

class HistoryLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HistoryLineEdit(QWidget* parent = 0);
    void setHistory(const QStringList &hist);
    QStringList history() const;

public slots:
    void addToHistory(const QString &s);
    void addToHistory();
    
protected:
    virtual void keyPressEvent(QKeyEvent* );
private:
    QCompleter *m_completer;
    QStringListModel *m_model;
};

#endif // HISTORYLINEEDIT_H
