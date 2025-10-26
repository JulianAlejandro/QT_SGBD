#include "treewidget.h"
#include <QAction>
#include <QMouseEvent>

TreeWidget::TreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    mActionNuevaTabla = new QAction(QIcon(":tabla"), "Nueva tabla", this);
    mActionEliminarTabla = new QAction(QIcon(":"), "Eliminar tabla", this);
    mActionRemoverBase = new QAction(QIcon(":"), "Remover Base", this);
    mSELECT_FROM = new QAction(QIcon(":"),"SELECT *FROM", this);

    addActions({mActionNuevaTabla, mActionEliminarTabla, mActionRemoverBase, mSELECT_FROM});

    connect(mActionNuevaTabla, &QAction::triggered, [&](){
        emit nuevaTabla();
    });
    connect(mActionEliminarTabla, &QAction::triggered, [&](){
        emit eliminarTabla();
    });
    connect(mActionRemoverBase, &QAction::triggered, [&](){
        emit removerBase();
    });
    connect(mSELECT_FROM, &QAction::triggered, [&](){
        emit selectFrom();
    });

}
