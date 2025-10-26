#ifndef AJUSTES_H
#define AJUSTES_H

#include <QString>
#include <QVariant>

#define GFONT "GFONT"
#define KFONTTYPE "KFONTTYPE"
#define KFONTSIZE "KFONTSIZE"

//carga de ajustes
QVariant Load(const QString &g, const QString &k, const QVariant &vd = QVariant());

//guarda ajustes
void Save(const QString &g, const QString &k, const QVariant &v);

#endif // AJUSTES_H
