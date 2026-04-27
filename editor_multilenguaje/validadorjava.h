#ifndef VALIDADORJAVA_H
#define VALIDADORJAVA_H

#include "validadorsintaxis.h"

class ValidadorJava : public ValidadorSintaxis
{
public:
    bool validarLinea(const QString &linea) override;
    QString mensajeDiagnostico(const QString &linea) override;
    QString nombreLenguaje() const override;
};

#endif // VALIDADORJAVA_H