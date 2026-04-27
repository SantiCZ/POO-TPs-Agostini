#include "validadorcpp.h"
#include <QRegularExpression>

// Reglas de validación básica para C++
// - Líneas que terminan con { o } son válidas (bloques)
// - Líneas que son comentarios son válidas
// - Instrucciones simples deben terminar en ;
// - Directivas de preprocesador son válidas
// - Líneas en blanco son válidas
// - Declaraciones de funciones/clases son válidas

bool ValidadorCpp::validarLinea(const QString &linea)
{
    QString l = linea.trimmed();

    // Línea en blanco: siempre válida
    if (l.isEmpty()) return true;

    // Comentarios de línea
    if (l.startsWith("//")) return true;

    // Directivas de preprocesador
    if (l.startsWith("#")) return true;

    // Líneas de bloque de apertura/cierre
    if (l == "{" || l == "}" || l.endsWith("{") || l == "};") return true;

    // Declaración de namespace, class, struct, enum
    QRegularExpression reDecl("^(namespace|class|struct|enum|public|private|protected|using|typedef)\\b");
    if (reDecl.match(l).hasMatch()) return true;

    // Línea que termina en ; (instrucción)
    if (l.endsWith(";")) return true;

    // Retorno sin ;
    QRegularExpression reReturn("^return\\s+");
    if (reReturn.match(l).hasMatch() && !l.endsWith(";")) return false;

    // Declaración de función (contiene paréntesis sin ;)
    if (l.contains("(") && l.contains(")") && !l.endsWith(";")) return true;

    // Si tiene contenido pero no coincide con ninguna regla -> sospechoso
    return false;
}

QString ValidadorCpp::mensajeDiagnostico(const QString &linea)
{
    QString l = linea.trimmed();

    if (l.isEmpty()) return "";

    if (!l.endsWith(";") && !l.endsWith("{") && !l.endsWith("}") &&
        !l.startsWith("//") && !l.startsWith("#") &&
        !l.contains("("))
    {
        return "⚠ C++: posiblemente falta ';' al final de la instrucción.";
    }

    if (l.startsWith("return") && !l.endsWith(";")) {
        return "⚠ C++: instrucción 'return' sin ';' al final.";
    }

    return "⚠ C++: línea con sintaxis no reconocida. Verificá paréntesis, llaves y punto y coma.";
}

QString ValidadorCpp::nombreLenguaje() const
{
    return "C++";
}
