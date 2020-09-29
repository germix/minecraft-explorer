#ifndef EDITBINARYVALUEDIALOG_H
#define EDITBINARYVALUEDIALOG_H

#include <QDialog>

namespace Ui {
class EditBinaryValueDialog;
}
class HexView;
class TreeItem;
class TreeModel;

class EditBinaryValueDialog : public QDialog
{
    Q_OBJECT
    Ui::EditBinaryValueDialog* ui;
    HexView* hexView;
    TreeModel* model;
    TreeItem* treeItem;
    QByteArray currentData;
public:
    explicit EditBinaryValueDialog(TreeModel* modelIn, TreeItem* treeItemIn, QWidget* parent = 0);
    ~EditBinaryValueDialog();
public:
    void updateLength();
private slots:
    void accept();
    void slotTextEdit_textChanged();
};

#endif // EDITBINARYVALUEDIALOG_H
