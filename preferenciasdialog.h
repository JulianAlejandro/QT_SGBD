#ifndef PREFERENCIASDIALOG_H
#define PREFERENCIASDIALOG_H

#include <QDialog>

#include <QFont>

namespace Ui {
class PreferenciasDialog;
}

class PreferenciasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferenciasDialog(QWidget *parent = nullptr);
    ~PreferenciasDialog();
signals:
    void aplicadoFuente(QFont);
private slots:
    void on_aplicar_clicked();

    void on_aceptar_clicked();

private:
    Ui::PreferenciasDialog *ui;
};

#endif // PREFERENCIASDIALOG_H
