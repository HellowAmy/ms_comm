/****************************************************************************
** Meta object code from reading C++ file 'net_connect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../net_connect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'net_connect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_net_connect_t {
    QByteArrayData data[10];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_net_connect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_net_connect_t qt_meta_stringdata_net_connect = {
    {
QT_MOC_LITERAL(0, 0, 11), // "net_connect"
QT_MOC_LITERAL(1, 12, 16), // "fa_connect_close"
QT_MOC_LITERAL(2, 29, 4), // "emit"
QT_MOC_LITERAL(3, 34, 15), // "fa_login_status"
QT_MOC_LITERAL(4, 50, 2), // "ok"
QT_MOC_LITERAL(5, 53, 11), // "fa_swap_txt"
QT_MOC_LITERAL(6, 65, 4), // "from"
QT_MOC_LITERAL(7, 70, 3), // "txt"
QT_MOC_LITERAL(8, 74, 13), // "fa_swap_files"
QT_MOC_LITERAL(9, 88, 8) // "filename"

    },
    "net_connect\0fa_connect_close\0emit\0"
    "fa_login_status\0ok\0fa_swap_txt\0from\0"
    "txt\0fa_swap_files\0filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_net_connect[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,
       5,    2,   38,    2, 0x06 /* Public */,
       8,    2,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::LongLong, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::LongLong, QMetaType::QString,    6,    9,

       0        // eod
};

void net_connect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<net_connect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fa_connect_close(); break;
        case 1: _t->fa_login_status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->fa_swap_txt((*reinterpret_cast< long long(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->fa_swap_files((*reinterpret_cast< long long(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (net_connect::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&net_connect::fa_connect_close)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (net_connect::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&net_connect::fa_login_status)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (net_connect::*)(long long , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&net_connect::fa_swap_txt)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (net_connect::*)(long long , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&net_connect::fa_swap_files)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject net_connect::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_net_connect.data,
    qt_meta_data_net_connect,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *net_connect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *net_connect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_net_connect.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int net_connect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void net_connect::fa_connect_close()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void net_connect::fa_login_status(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void net_connect::fa_swap_txt(long long _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void net_connect::fa_swap_files(long long _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
