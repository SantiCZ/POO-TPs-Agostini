#include "validadorjava.h"
#include <QRegularExpression>

bool ValidadorJava::validarLinea(const QString &linea)
{
    QString l = linea.trimmed();

    if (l.isEmpty()) return true;

    // Comentarios
    if (l.startsWith("//") || l.startsWith("/*") || l.startsWith("*")) return true;

    // Anotaciones (@Override, @SuppressWarnings, etc.)
    if (l.startsWith("@")) return true;

    // Bloques de apertura/cierre
    if (l == "{" || l == "}" || l == "};" || l.endsWith("{")) return true;

    // Package e import
    QRegularExpression reImport("^(package|import)\\s+");
    if (reImport.match(l).hasMatch()) return l.endsWith(";");

    // Declaraciones de clase/interface/enum
    QRegularExpression reDecl("^(public|private|protected|static|final|abstract|class|interface|enum)\\b");
    if (reDecl.match(l).hasMatch()) return true;

    // Instrucciones que terminan en ;
    if (l.endsWith(";")) return true;

    // Declaración de método (tiene paréntesis y llave de apertura o sin ;)
    if (l.contains("(") && l.contains(")") && !l.endsWith(";")) return true;

    return false;
}

QString ValidadorJava::mensajeDiagnostico(const QString &linea)
{
    QString l = linea.trimmed();

    QRegularExpression reImport("^(package|import)\\s+");
    if (reImport.match(l).hasMatch() && !l.endsWith(";")) {
        return "⚠ Java: instrucción 'import' o 'package' sin ';' al final.";
    }

    if (!l.endsWith(";") && !l.endsWith("{") && !l.endsWith("}") &&
        !l.startsWith("//") && !l.startsWith("@") && !l.startsWith("*"))
    {
        return "⚠ Java: posiblemente falta ';' al final de la instrucción.";
    }

    return "⚠ Java: línea con sintaxis no reconocida. Verificá el modificador de acceso y el punto y coma.";
}

QString ValidadorJava::nombreLenguaje() const
{
    return "Java";
}
