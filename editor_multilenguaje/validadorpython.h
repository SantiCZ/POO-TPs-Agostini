#ifndef VALIDADORPYTHON_H
#define VALIDADORPYTHON_H

#include "validadorsintaxis.h"

class ValidadorPython : public ValidadorSintaxis
{
public:
    bool validarLinea(const QString &linea) override;
    QString mensajeDiagnostico(const QString &linea) override;
    QString nombreLenguaje() const override;
};

#endif // VALIDADORPYTHON_H