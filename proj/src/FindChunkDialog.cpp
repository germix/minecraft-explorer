#include "FindChunkDialog.h"
#include "ui_FindChunkDialog.h"

FindChunkDialog::FindChunkDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FindChunkDialog)
{
    ui->setupUi(this);

    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->txtChunkX, SIGNAL(textChanged(QString)), this, SLOT(slotChunkTextEdit_textChanged(QString)));
    connect(ui->txtChunkZ, SIGNAL(textChanged(QString)), this, SLOT(slotChunkTextEdit_textChanged(QString)));
    connect(ui->txtBlockX, SIGNAL(textChanged(QString)), this, SLOT(slotBlockTextEdit_textChanged(QString)));
    connect(ui->txtBlockZ, SIGNAL(textChanged(QString)), this, SLOT(slotBlockTextEdit_textChanged(QString)));
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

void FindChunkDialog::slotChunkTextEdit_textChanged(const QString& text)
{
    Q_UNUSED(text);
    bool oldState;

    oldState = ui->txtBlockX->blockSignals(true);
    ui->txtBlockX->setText("");
    ui->txtBlockX->blockSignals(oldState);

    oldState = ui->txtBlockZ->blockSignals(true);
    ui->txtBlockZ->setText("");
    ui->txtBlockZ->blockSignals(oldState);
}

void FindChunkDialog::slotBlockTextEdit_textChanged(const QString& text)
{
    Q_UNUSED(text);
    bool oldState;

    oldState = ui->txtChunkX->blockSignals(true);
    ui->txtChunkX->setText(QString::number(ui->txtBlockX->text().toInt()>>4));
    ui->txtChunkX->blockSignals(oldState);

    oldState = ui->txtChunkZ->blockSignals(true);
    ui->txtChunkZ->setText(QString::number(ui->txtBlockZ->text().toInt()>>4));
    ui->txtChunkZ->blockSignals(oldState);
}


