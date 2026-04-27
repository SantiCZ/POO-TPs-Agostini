#include "validadorpython.h"
#include <QRegularExpression>

bool ValidadorPython::validarLinea(const QString &linea)
{
    QString l = linea.trimmed();

    if (l.isEmpty()) return true;

    // Comentarios
    if (l.startsWith("#")) return true;

    // Strings multilinea (docstrings)
    if (l.startsWith("\"\"\"") || l.startsWith("'''")) return true;

    // Python NO usa ; al final (si lo tiene, es sospechoso pero válido)
    // Palabras clave que abren bloque deben terminar en :
    QRegularExpression reBloque("^(if|elif|else|for|while|def|class|try|except|finally|with|async def|async for|async with)\\b");
    if (reBloque.match(l).hasMatch()) {
        return l.endsWith(":");
    }

    // Import statements
    QRegularExpression reImport("^(import|from)\\s+");
    if (reImport.match(l).hasMatch()) return true;

    // Asignaciones y expresiones
    QRegularExpression reAsign("^[a-zA-Z_][a-zA-Z0-9_]*\\s*(=|\\+=|-=|\\*=|/=)");
    if (reAsign.match(l).hasMatch()) return true;

    // Return, pass, break, continue
    QRegularExpression reSimple("^(return|pass|break|continue|raise|yield|del|assert|global|nonlocal)\\b");
    if (reSimple.match(l).hasMatch()) return true;

    // Llamada a función
    if (l.contains("(") && l.contains(")")) return true;

    // Print sin paréntesis (Python 2 error común)
    QRegularExpression rePrint2("^print\\s+[^(]");
    if (rePrint2.match(l).hasMatch()) return false;

    return false;
}

QString ValidadorPython::mensajeDiagnostico(const QString &linea)
{
    QString l = linea.trimmed();

    QRegularExpression reBloque("^(if|elif|else|for|while|def|class|try|except|finally|with)\\b");
    if (reBloque.match(l).hasMatch() && !l.endsWith(":")) {
        return "⚠ Python: falta ':' al final del bloque '" + l.split(" ").first() + "'.";
    }

    QRegularExpression rePrint2("^print\\s+[^(]");
    if (rePrint2.match(l).hasMatch()) {
        return "⚠ Python: usá print() con paréntesis (sintaxis Python 3).";
    }

    return "⚠ Python: línea no reconocida. Verificá indentación y estructura del bloque.";
}

QString ValidadorPython::nombreLenguaje() const
{
    return "Python";
}
