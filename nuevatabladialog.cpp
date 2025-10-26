#include "nuevatabladialog.h"
#include "ui_nuevatabladialog.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QAction>

NuevaTablaDialog::NuevaTablaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NuevaTablaDialog)
{
    ui->setupUi(this);
    auto agregar = new QAction("Agregar", this);
    auto eliminar = new QAction("Eliminar", this);
    ui->tableWidget->addActions({agregar, eliminar});

    auto nuevoComboBox = [&](QWidget *parent){
        auto combo = new QComboBox(parent);
        combo->addItems({"INTEGER", "DOUBLE", "TEXT", "DATE", "TIME", "BOOL"});
        return combo;
    };

    connect(agregar, &QAction::triggered, [&](){
        int nfilas = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(nfilas);
        ui->tableWidget->setCellWidget(nfilas, 1, nuevoComboBox(this));
        ui->tableWidget->setCellWidget(nfilas, 2, new QRadioButton(this));
        ui->tableWidget->setCellWidget(nfilas, 3, new QCheckBox(this));
        ui->tableWidget->setCellWidget(nfilas, 4, new QCheckBox(this));
    });
    connect(eliminar, &QAction::triggered, [&](){
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    });
}

NuevaTablaDialog::~NuevaTablaDialog()
{
    delete ui;
}

QString NuevaTablaDialog::resultado() const
{
    QString res = QString("CREATE TABLE IF NOT EXISTS %1 (").arg(ui->nombre->text());
    int nfilas = ui->tableWidget->rowCount();
    for (int i = 0; i < nfilas; i++){
        auto nombre = ui->tableWidget->item(i, 0)->text();
        auto tipo = static_cast<QComboBox *>(ui->tableWidget->cellWidget(i, 1))->currentText();
        bool pk = static_cast<QRadioButton *>(ui->tableWidget->cellWidget(i, 2))->isChecked();
        bool nn = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 3))->isChecked();
        bool ai = static_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 4))->isChecked();

        res += nombre + " " + tipo;
        if (pk){
            res += " PRIMARY KEY";
        }
        if (ai){
            res += " AUTOINCREMENT";
        }
        if (nn){
            res += " NOT NULL";
        }
        res += ",";
    }
    res[res.size()-1] = ')';
    return res;
}

void NuevaTablaDialog::on_aceptar_clicked()
{
    accept();
}


void NuevaTablaDialog::on_cancelar_clicked()
{
    reject();
}

