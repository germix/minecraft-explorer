#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT
    Ui::RenameDialog* ui;
public:
    explicit RenameDialog(const QString& initialName, QWidget* parent = 0);
    ~RenameDialog();
public:
    QString getName() const;
private slots:
    void accept();
};

#endif // RENAMEDIALOG_H
