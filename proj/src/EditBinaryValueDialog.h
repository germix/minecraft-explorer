#ifndef EDITBINARYVALUEDIALOG_H
#define EDITBINARYVALUEDIALOG_H

#include <QDialog>

namespace Ui {
class EditBinaryValueDialog;
}
class HexView;
class TreeItem;
class TreeModel;
class QFile;

class EditBinaryValueDialog : public QDialog
{
    Q_OBJECT
    Ui::EditBinaryValueDialog* ui;
    HexView* hexView;
    TreeModel* model;
    TreeItem* treeItem;
    QByteArray currentData;
    QStringList dataFilters;
public:
    explicit EditBinaryValueDialog(TreeModel* modelIn, TreeItem* treeItemIn, QWidget* parent = 0);
    ~EditBinaryValueDialog();
private:
    void updateLength();
    void importTextData(QFile& file);
    void importBinaryData(QFile& file);
    void exportAsTextData(QFile& file);
    void exportAsBinaryData(QFile& file);
    bool dataToText(QByteArray data, QString* output);
    void updateDataFromText();
    void updateTextFromData();
private slots:
    void accept();
    void slotImport();
    void slotExport();
    void slotTextEdit_textChanged();
};

#endif // EDITBINARYVALUEDIALOG_H
