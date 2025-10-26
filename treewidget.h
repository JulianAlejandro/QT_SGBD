#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>

class QAction;

class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    TreeWidget(QWidget *parent = nullptr);
signals:
    void nuevaTabla();
    void eliminarTabla();
    void removerBase();
    void selectFrom();
private:
    QAction *mActionNuevaTabla;
    QAction *mActionEliminarTabla;
    QAction *mActionRemoverBase;
    QAction *mSELECT_FROM;

};

#endif // TREEWIDGET_H
