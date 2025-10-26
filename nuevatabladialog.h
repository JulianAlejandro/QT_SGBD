#ifndef NUEVATABLADIALOG_H
#define NUEVATABLADIALOG_H

#include <QDialog>

namespace Ui {
class NuevaTablaDialog;
}

class NuevaTablaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NuevaTablaDialog(QWidget *parent = nullptr);
    ~NuevaTablaDialog();
    QString resultado() const;

private slots:
    void on_aceptar_clicked();

    void on_cancelar_clicked();

private:
    Ui::NuevaTablaDialog *ui;
};

#endif // NUEVATABLADIALOG_H
