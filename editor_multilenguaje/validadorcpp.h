#ifndef VALIDADORCPP_H
#define VALIDADORCPP_H

#include "validadorsintaxis.h"

class ValidadorCpp : public ValidadorSintaxis
{
public:
    bool validarLinea(const QString &linea) override;
    QString mensajeDiagnostico(const QString &linea) override;
    QString nombreLenguaje() const override;
};

#endif // VALIDADORCPP_H