#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "preferenciasdialog.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QPrintDialog>
#include <QPrinter>

#include "findreplacedialog.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QSqlQueryModel>
#include "ajustes.h"

#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>

#include "nuevatabladialog.h"

//nuevo nombre
using TreeItem = QTreeWidgetItem;
using ListItem = QListWidgetItem;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mQueryModel = new QSqlQueryModel(this);
    ui->tableView->setModel(mQueryModel);
    //centralWidget()->layout()->setMargin(0); esto h

    auto vfont = Load(GFONT, KFONTTYPE, ui->textEdit->font());
    ui->textEdit->setFont(vfont.value<QFont>());
    setAcceptDrops(true);

    connect(ui->treeWidget, &TreeWidget::nuevaTabla, [&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una base de datos");            return;
        }
        auto padre = itm->parent();
        if(padre != nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una base de datos");
            return;
        }
        //se trata de una base de datos
        NuevaTablaDialog D(this);
        if(D.exec() == QDialog::Rejected){
            return;
        }
        //agregar el cdigo SQL para generar esa tabla
        ui->textEdit->setPlainText(D.resultado());

    });
    connect(ui->treeWidget, &TreeWidget::eliminarTabla, [&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una tabla de datos");
            return;
        }
        auto padre = itm->parent();
        if(padre == nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una tabla de datos");
            return;
        }
        ui->textEdit->setPlainText(QString("DROP TABLE %1").arg(itm->text(0)));

    });
    connect(ui->treeWidget, &TreeWidget::removerBase, [&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una base de datos");
            return;
        }
        auto padre = itm->parent();
        if(padre != nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una base de datos");
            return;
        }
        delete itm;
    });
    connect(ui->treeWidget, &TreeWidget::selectFrom, [&](){
        auto itm = ui->treeWidget->currentItem();
        if(itm == nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una tabla de datos");
            return;
        }
        auto padre = itm->parent();
        if(padre == nullptr){
            //no es una base de datos
            QMessageBox::warning(this, "Advertencia", "Esta funcion solo esta permitida cuando se selecciona una tabla de datos");
            return;
        }
        ui->textEdit->setPlainText(QString("SELECT * FROM %1").arg(itm->text(0)));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dragEnterEvent(QDragEnterEvent *e){
    e->accept();
}
void MainWindow::dragLeaveEvent(QDragLeaveEvent *e){
    e->accept();
}
void MainWindow::dragMoveEvent(QDragMoveEvent *e){
    e->accept();
}
void MainWindow::dropEvent(QDropEvent *e){
    auto datos = e->mimeData();
    auto urls = datos->urls();
    for (auto u : urls){
        auto nombre = u.toLocalFile();
        auto baseitem = new TreeItem;
        baseitem->setIcon(0, QIcon(":base"));
        baseitem->setText(0, nombre);


        mDb.setDatabaseName(nombre);
        if (!mDb.open()){
            QMessageBox::critical(this, "Error", QString("Error: No se ha podido abrir la base de datos: %1").arg(nombre));
            return;
        }
        QSqlQuery qry;
        qry.exec("SELECT tbl_name FROM sqlite_master WHERE type LIKE 'table'");
        while (qry.next()){
            auto tablaitem = new TreeItem;
            tablaitem->setIcon(0, QIcon(":tabla"));
            tablaitem->setText(0, qry.value(0).toString());
            baseitem->addChild(tablaitem);
        }
        ui->treeWidget->addTopLevelItem(baseitem);
    }
}


void MainWindow::on_actionAbrir_base_triggered()
{
    auto nombre = QFileDialog::getOpenFileName(this, "Abrir base de datos", QDir::currentPath(), "Base de datos (*.db);;" \
                                                                                                 "Cualquier tipo (*.*)");
    if (nombre.isEmpty())
        return;

    auto baseitem = new TreeItem;
    baseitem->setIcon(0, QIcon(":base"));
    baseitem->setText(0, nombre);


    mDb.setDatabaseName(nombre);
    if (!mDb.open()){
        QMessageBox::critical(this, "Error", QString("Error: No se ha podido abrir la base de datos: %1").arg(nombre));
        return;
    }
    QSqlQuery qry;
    qry.exec("SELECT tbl_name FROM sqlite_master WHERE type LIKE 'table'");
    while (qry.next()){
        auto tablaitem = new TreeItem;
        tablaitem->setIcon(0, QIcon(":tabla"));
        tablaitem->setText(0, qry.value(0).toString());
        baseitem->addChild(tablaitem);
    }
    ui->treeWidget->addTopLevelItem(baseitem);
}


void MainWindow::on_actionGuardar_base_triggered()
{
    auto nombre = QFileDialog::getSaveFileName(this, "Guardar base de datos", QDir::currentPath(), "Base de datos (*.db);;" \
                                               "Cualquier tipo (*.*)");
    if (nombre.isEmpty())
        return;

    auto baseitem = new TreeItem;
    baseitem->setIcon(0, QIcon(":base"));
    baseitem->setText(0, nombre);
    ui->treeWidget->addTopLevelItem(baseitem);

    mDb.setDatabaseName(nombre);
    if (!mDb.open()){
        QMessageBox::critical(this, "Error", QString("Error: No se ha podido guardar la base de datos: %1").arg(nombre));
    }
}


void MainWindow::on_actionAbrir_SQL_triggered()
{
    auto nombre = QFileDialog::getOpenFileName(this, "Abrir SQL", QDir::currentPath(), "Archivo SQL (*.sql);;Cualquier tipo (*.*)");

    if (nombre.isEmpty()){
        return;
    }
    QFile F(nombre);
    if(!F.open(QIODevice::Text | QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Error", QString("Error: %1").arg(F.errorString()));
        return;
    }
    QTextStream S(&F);
    ui->textEdit->setText(S.readAll());
    F.close();
}


void MainWindow::on_actionGuardar_SQL_triggered()
{
    auto nombre = QFileDialog::getSaveFileName(this, "Guardar SQL", QDir::currentPath(), "Archivo SQL (*.sql);;Cualquier tipo (*.*)");

    if (nombre.isEmpty()){
        return;
    }
    QFile F(nombre);
    if(!F.open(QIODevice::Text | QIODevice::WriteOnly)){
        QMessageBox::critical(this, "Error", QString("Error: %1").arg(F.errorString()));
        return;
    }
    QTextStream S (&F);
    S << ui->textEdit->toPlainText();
    F.flush();
    F.close();
}


void MainWindow::on_actionImprimir_triggered()
{
    QPrintDialog P(this);
    if(P.exec() == QDialog::Rejected){
        return;
    }
    ui->textEdit->print(P.printer());
}


void MainWindow::on_actionQuitar_triggered()
{
    close();
}


void MainWindow::on_actionCopiar_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionCortar_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionPegar_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionEjecutar_triggered()
{
    auto sql = ui->textEdit->toPlainText();
    if(sql.startsWith("select", Qt::CaseInsensitive)){
        mQueryModel->setQuery(sql);
    }else if(sql.startsWith("create table", Qt::CaseInsensitive)){
        QSqlQuery qry;
        if(!qry.exec(sql)){
            ui->listWidget->addItem(new ListItem(QIcon(":quitar"), sql));
            return;
        }
        ui->listWidget->addItem(new ListItem(QIcon(":tabla"), sql));
        int i = 0;
        auto base = mDb.databaseName();
        int tam = ui->treeWidget->topLevelItemCount();
        for(i = 0; i < tam; i++){
            if(ui->treeWidget->topLevelItem(i)->text(0).toLower() == base.toLower()){
                break;
            }
        }
        auto baseitem = ui->treeWidget->topLevelItem(i);


        //buscando el nombre de la tabla
        auto createtable = QString("create table ");
        if(sql.contains("if not exists", Qt::CaseInsensitive)){
            createtable = QString("create table if not exists ");
        }

        int pos1 = createtable.size();
        int pos2 = sql.indexOf("(", pos1);
        auto tabla = sql.mid(pos1, pos2 - pos1);
        tabla = tabla.mid(0, tabla.indexOf(" "));

        tam = baseitem->childCount();
        for (i = 0; i < tam; i++){
            if (baseitem->child(i)->text(0).toLower() == tabla.toLower()){
                return;
            }

        }
        auto tablaitem = new TreeItem;
        tablaitem->setIcon(0, QIcon(":tabla"));
        tablaitem->setText(0, tabla);
        baseitem->addChild(tablaitem);

    }else if(sql.startsWith("drop table", Qt::CaseInsensitive)){
        QSqlQuery qry;
        if(!qry.exec(sql)){
            ui->listWidget->addItem(new ListItem(QIcon(":quitar"), sql));
            return;
        }
        ui->listWidget->addItem(new ListItem(QIcon(":tabla"), sql));

        int i = 0;
        auto base = mDb.databaseName();
        int tam = ui->treeWidget->topLevelItemCount();
        for(i = 0; i < tam; i++){
            if(ui->treeWidget->topLevelItem(i)->text(0).toLower() == base.toLower()){
                break;
            }
        }
        auto baseitem = ui->treeWidget->topLevelItem(i);
        tam = baseitem->childCount();
        auto droptable = QString("drop table");
        auto tabla = sql.mid(droptable.size());
        tabla = tabla.mid(1);
        for ( i = 0; i < tam; i++){
            if(baseitem->child(i)->text(0).toLower() == tabla.toLower()){
                break;
            }
        }
        baseitem->removeChild(baseitem->child(i));
    }else{
        QSqlQuery qry;
        if(!qry.exec(sql)){
            ui->listWidget->addItem(new ListItem(QIcon(":quitar"), sql));
            return;
        }
        ui->listWidget->addItem(new ListItem(QIcon(":tabla"), sql));
    }
}


void MainWindow::on_actionPreferencias_triggered()
{
    PreferenciasDialog p(this);
    connect(&p, &PreferenciasDialog::aplicadoFuente, [&](const QFont &f){
        ui->textEdit->setFont(f);
        Save(GFONT, KFONTTYPE, f);
        //Svae(GFONT, KFONTSIZE, f.pointSizeF());
    });
    if (p.exec() == QDialog::Rejected){
        return;
    }
}


void MainWindow::on_actionManual_triggered()
{
    //TODO
}


void MainWindow::on_actionAcerca_de_Qt_triggered()
{
    QMessageBox::about(this, "SQLiteManager", "Autor: julian Compañia julian Fecha: 3/8/2025 Software: SQLITEMANAGER");
}


void MainWindow::on_actionBuscar_y_remplazar_triggered()
{
    FindReplaceDialog F(this);
    F.setTextEdit(ui->textEdit);
    F.exec();
}


void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{
    if(index.parent().isValid()){
        return;
    }
    auto nombre = index.data().toString();
    mDb.setDatabaseName(nombre);
    if(!mDb.open()){
        QMessageBox::critical(this, "Error", QString("Error: No se ha podido guardar la base de datos: %1").arg(nombre));
    }
}



