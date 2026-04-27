/****************************************************************************
** Meta object code from reading C++ file 'editorprincipal.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../editorprincipal.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editorprincipal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13EditorDeTextoE_t {};
} // unnamed namespace

template <> constexpr inline auto EditorDeTexto::qt_create_metaobjectdata<qt_meta_tag_ZN13EditorDeTextoE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "EditorDeTexto",
        "lineaAbandonada",
        "",
        "numeroLinea",
        "contenidoLinea",
        "teclaPresionada",
        "key",
        "Qt::KeyboardModifiers",
        "mods",
        "mousePresionado",
        "QPoint",
        "pos",
        "Qt::MouseButton",
        "btn"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'lineaAbandonada'
        QtMocHelpers::SignalData<void(int, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 },
        }}),
        // Signal 'teclaPresionada'
        QtMocHelpers::SignalData<void(int, Qt::KeyboardModifiers)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { 0x80000000 | 7, 8 },
        }}),
        // Signal 'mousePresionado'
        QtMocHelpers::SignalData<void(const QPoint &, Qt::MouseButton)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 }, { 0x80000000 | 12, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<EditorDeTexto, qt_meta_tag_ZN13EditorDeTextoE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject EditorDeTexto::staticMetaObject = { {
    QMetaObject::SuperData::link<QPlainTextEdit::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13EditorDeTextoE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13EditorDeTextoE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13EditorDeTextoE_t>.metaTypes,
    nullptr
} };

void EditorDeTexto::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<EditorDeTexto *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->lineaAbandonada((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->teclaPresionada((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Qt::KeyboardModifiers>>(_a[2]))); break;
        case 2: _t->mousePresionado((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Qt::MouseButton>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (EditorDeTexto::*)(int , const QString & )>(_a, &EditorDeTexto::lineaAbandonada, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (EditorDeTexto::*)(int , Qt::KeyboardModifiers )>(_a, &EditorDeTexto::teclaPresionada, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (EditorDeTexto::*)(const QPoint & , Qt::MouseButton )>(_a, &EditorDeTexto::mousePresionado, 2))
            return;
    }
}

const QMetaObject *EditorDeTexto::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditorDeTexto::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13EditorDeTextoE_t>.strings))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int EditorDeTexto::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void EditorDeTexto::lineaAbandonada(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void EditorDeTexto::teclaPresionada(int _t1, Qt::KeyboardModifiers _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void EditorDeTexto::mousePresionado(const QPoint & _t1, Qt::MouseButton _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}
namespace {
struct qt_meta_tag_ZN15EditorPrincipalE_t {};
} // unnamed namespace

template <> constexpr inline auto EditorPrincipal::qt_create_metaobjectdata<qt_meta_tag_ZN15EditorPrincipalE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "EditorPrincipal",
        "cambiarLenguaje",
        "",
        "lenguaje",
        "validarLinea",
        "numeroLinea",
        "contenido",
        "exportarJpg",
        "manejarTecla",
        "key",
        "Qt::KeyboardModifiers",
        "mods",
        "manejarMouse",
        "QPoint",
        "pos",
        "Qt::MouseButton",
        "btn",
        "actualizarBarraEstado",
        "nuevoArchivo",
        "limpiarErrores"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'cambiarLenguaje'
        QtMocHelpers::SlotData<void(const QString &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'validarLinea'
        QtMocHelpers::SlotData<void(int, const QString &)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { QMetaType::QString, 6 },
        }}),
        // Slot 'exportarJpg'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'manejarTecla'
        QtMocHelpers::SlotData<void(int, Qt::KeyboardModifiers)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { 0x80000000 | 10, 11 },
        }}),
        // Slot 'manejarMouse'
        QtMocHelpers::SlotData<void(const QPoint &, Qt::MouseButton)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 13, 14 }, { 0x80000000 | 15, 16 },
        }}),
        // Slot 'actualizarBarraEstado'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'nuevoArchivo'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'limpiarErrores'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<EditorPrincipal, qt_meta_tag_ZN15EditorPrincipalE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject EditorPrincipal::staticMetaObject = { {
    QMetaObject::SuperData::link<Pantalla::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15EditorPrincipalE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15EditorPrincipalE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15EditorPrincipalE_t>.metaTypes,
    nullptr
} };

void EditorPrincipal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<EditorPrincipal *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->cambiarLenguaje((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->validarLinea((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->exportarJpg(); break;
        case 3: _t->manejarTecla((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Qt::KeyboardModifiers>>(_a[2]))); break;
        case 4: _t->manejarMouse((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Qt::MouseButton>>(_a[2]))); break;
        case 5: _t->actualizarBarraEstado(); break;
        case 6: _t->nuevoArchivo(); break;
        case 7: _t->limpiarErrores(); break;
        default: ;
        }
    }
}

const QMetaObject *EditorPrincipal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EditorPrincipal::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15EditorPrincipalE_t>.strings))
        return static_cast<void*>(this);
    return Pantalla::qt_metacast(_clname);
}

int EditorPrincipal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Pantalla::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
