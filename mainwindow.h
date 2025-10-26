#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);

private slots:
    void on_actionAbrir_base_triggered();
    void on_actionGuardar_base_triggered();
    void on_actionAbrir_SQL_triggered();
    void on_actionGuardar_SQL_triggered();
    void on_actionImprimir_triggered();
    void on_actionQuitar_triggered();
    void on_actionCopiar_triggered();
    void on_actionCortar_triggered();
    void on_actionPegar_triggered();
    void on_actionEjecutar_triggered();
    void on_actionPreferencias_triggered();
    void on_actionManual_triggered();
    void on_actionAcerca_de_Qt_triggered();
    void on_actionBuscar_y_remplazar_triggered();

    void on_treeWidget_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase mDb;
    QSqlQueryModel *mQueryModel;
};
#endif // MAINWINDOW_H
