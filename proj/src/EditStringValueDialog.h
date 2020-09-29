#ifndef EDITSTRINGVALUEDIALOG_H
#define EDITSTRINGVALUEDIALOG_H

#include <QDialog>

namespace Ui {
class EditStringValueDialog;
}
class TreeModel;
class TreeItemNbtTagString;

class EditStringValueDialog : public QDialog
{
    Q_OBJECT
    Ui::EditStringValueDialog* ui;
    TreeModel* model;
    TreeItemNbtTagString* treeItem;
public:
    explicit EditStringValueDialog(TreeModel* modelIn, TreeItemNbtTagString* treeItemIn, QWidget* parent = 0);
    ~EditStringValueDialog();
private slots:
    void accept();
};

#endif // EDITSTRINGVALUEDIALOG_H
