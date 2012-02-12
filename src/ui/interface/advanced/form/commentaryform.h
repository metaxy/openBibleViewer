#ifndef COMMENTARYFORM_H
#define COMMENTARYFORM_H

#include <QWidget>
#include "webviewform.h"
#include "src/module/commentary/webcommentary.h"
namespace Ui {
class CommentaryForm;
}

class CommentaryForm : public WebViewForm
{
    Q_OBJECT
    
public:
    explicit CommentaryForm(QWidget *parent = 0);
    ~CommentaryForm();

    void init();

    void restore(const QString &key);
    void save();

    void pharseUrl(QString string);
    void pharseUrl(VerseUrl url);

    Form::FormType type() const;
    void activated();
private:
    bool loaded();
private slots:
    void changeLocation();
private:
    Ui::CommentaryForm *ui;
    WebCommentary *m_com;
    VerseUrl m_url;
};

#endif // COMMENTARYFORM_H
