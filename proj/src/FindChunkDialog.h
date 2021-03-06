#ifndef FINDCHUNKDIALOG_H
#define FINDCHUNKDIALOG_H

#include <QDialog>

namespace Ui {
class FindChunkDialog;
}

class FindChunkDialog : public QDialog
{
    Q_OBJECT
    Ui::FindChunkDialog* ui;
public:
    explicit FindChunkDialog(QWidget* parent = 0);
    ~FindChunkDialog();
public:
    int getChunkX() const;
    int getChunkZ() const;
private slots:
    void slotChunkTextEdit_textChanged(const QString& text);
    void slotBlockTextEdit_textChanged(const QString& text);
};

#endif // FINDCHUNKDIALOG_H
