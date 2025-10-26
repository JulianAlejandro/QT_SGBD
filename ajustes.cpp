#include "ajustes.h"
#include <QSettings>

QVariant Load(const QString &g, const QString &k, const QVariant &vd){
    QSettings S;
    S.beginGroup(g);
    auto val = S.value(k, vd);
    S.endGroup();
    return val;
}

void Save(const QString &g, const QString &k, const QVariant &v){
    QSettings S;
    S.beginGroup(g);
    S.setValue(k, v);
    S.endGroup();
}
