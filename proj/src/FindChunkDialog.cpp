#include "FindChunkDialog.h"
#include "ui_FindChunkDialog.h"

FindChunkDialog::FindChunkDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FindChunkDialog)
{
    ui->setupUi(this);

    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
}

FindChunkDialog::~FindChunkDialog()
{
    delete ui;
}

int FindChunkDialog::getChunkX() const
{
    return ui->txtChunkX->text().toInt();
}

int FindChunkDialog::getChunkZ() const
{
    return ui->txtChunkZ->text().toInt();
}
