#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT
    Ui::FindDialog* ui;
public:
    explicit FindDialog(QWidget* parent = 0);
    ~FindDialog();
public:
    QString getName() const;
    QString getValue() const;
private slots:
    void slotTextEditName_textChanged(const QString& text);
    void slotTextEditValue_textChanged(const QString& text);
};

#endif // FINDDIALOG_H
