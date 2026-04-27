#ifndef VALIDADORSINTAXIS_H
#define VALIDADORSINTAXIS_H

#include <QString>

// ============================================================
// ValidadorSintaxis: clase abstracta para validadores de código
// Cada lenguaje implementa su propia lógica de validación.
// ============================================================
class ValidadorSintaxis
{
public:
    virtual ~ValidadorSintaxis() = default;

    // Valida una línea de código. Retorna true si es válida.
    virtual bool validarLinea(const QString &linea) = 0;

    // Retorna un mensaje de diagnóstico amigable para el usuario
    virtual QString mensajeDiagnostico(const QString &linea) = 0;

    // Nombre del lenguaje para mostrar en la UI
    virtual QString nombreLenguaje() const = 0;
};

#endif // VALIDADORSINTAXIS_H
