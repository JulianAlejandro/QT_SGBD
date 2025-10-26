#include "ajustes.h"
#include "preferenciasdialog.h"
#include "ui_preferenciasdialog.h"

PreferenciasDialog::PreferenciasDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferenciasDialog)
{
    ui->setupUi(this);
    auto vfont = Load(GFONT, KFONTTYPE);
    ui->fontComboBox->setCurrentFont(vfont.value<QFont>());
    ui->doubleSpinBox->setValue(vfont.value<QFont>().pixelSize());
}

PreferenciasDialog::~PreferenciasDialog()
{
    delete ui;
}

void PreferenciasDialog::on_aplicar_clicked()
{
    auto f = ui->fontComboBox->currentFont();
    f.setPointSizeF(ui->doubleSpinBox->value());
    emit aplicadoFuente(f);
}


void PreferenciasDialog::on_aceptar_clicked()
{
    on_aplicar_clicked();
    accept();
}

