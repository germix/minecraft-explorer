#ifndef EDITNUMERICVALUEDIALOG_H
#define EDITNUMERICVALUEDIALOG_H

#include <QDialog>

namespace Ui {
class EditNumericValueDialog;
}
class TreeModel;
class TreeItem;

class EditNumericValueDialog : public QDialog
{
    Q_OBJECT
    Ui::EditNumericValueDialog* ui;
    TreeModel* model;
    TreeItem* treeItem;
public:
    explicit EditNumericValueDialog(TreeModel* modelIn, TreeItem* treeItemIn, QWidget* parent = 0);
    ~EditNumericValueDialog();
private slots:
    void accept();
};

#endif // EDITNUMERICVALUEDIALOG_H
