#ifndef EXPORTADORJPG_H
#define EXPORTADORJPG_H

#include <QString>

class ExportadorJpg
{
public:
    ExportadorJpg();
    QString exportar(const QString &codigo, const QString &lenguaje, const QString &rutaDestino);
};

#endif // EXPORTADORJPG_H