#include "TrabajoPractico.h"

QString TrabajoPractico::estadoToString(EstadoTP e) {
    switch (e) {
        case EstadoTP::Pendiente:  return "Pendiente";
        case EstadoTP::EnCurso:    return "En curso";
        case EstadoTP::Entregado:  return "Entregado";
        case EstadoTP::Vencido:    return "Vencido";
    }
    return "Pendiente";
}

EstadoTP TrabajoPractico::estadoFromString(const QString& s) {
    if (s == "En curso")   return EstadoTP::EnCurso;
    if (s == "Entregado")  return EstadoTP::Entregado;
    if (s == "Vencido")    return EstadoTP::Vencido;
    return EstadoTP::Pendiente;
}

QString TrabajoPractico::prioridadToString(PrioridadTP p) {
    switch (p) {
        case PrioridadTP::Alta:  return "Alta";
        case PrioridadTP::Media: return "Media";
        case PrioridadTP::Baja:  return "Baja";
    }
    return "Media";
}

PrioridadTP TrabajoPractico::prioridadFromString(const QString& s) {
    if (s == "Alta") return PrioridadTP::Alta;
    if (s == "Baja") return PrioridadTP::Baja;
    return PrioridadTP::Media;
}
