#ifndef NEWTAGDIALOG_H
#define NEWTAGDIALOG_H
#include <QDialog>
#include <QModelIndex>

namespace Ui {
class NewTagDialog;
}
class TreeModel;

class NewTagDialog : public QDialog
{
    Q_OBJECT
    Ui::NewTagDialog* ui;
    TreeModel* model;
    int tagType;
    QModelIndex parentIndex;
public:
    explicit NewTagDialog(TreeModel* modelIn, int tagTypeIn, QModelIndex& parentIndexIn, QWidget* parent = 0);
    ~NewTagDialog();
public:
    QString getName() const;
private slots:
    void accept();
    void slotTextEdit_textEdited(const QString& text);
};

#endif // NEWTAGDIALOG_H
