/****************************************************************************
** Meta object code from reading C++ file 'IOLabel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "IOLabel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IOLabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IOLabel_t {
    QByteArrayData data[7];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IOLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IOLabel_t qt_meta_stringdata_IOLabel = {
    {
QT_MOC_LITERAL(0, 0, 7), // "IOLabel"
QT_MOC_LITERAL(1, 8, 13), // "sig_ZeroToOne"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "sig_OneToZero"
QT_MOC_LITERAL(4, 37, 11), // "slot_IOInit"
QT_MOC_LITERAL(5, 49, 13), // "slot_IOJerkOn"
QT_MOC_LITERAL(6, 63, 14) // "slot_IOJerkOff"

    },
    "IOLabel\0sig_ZeroToOne\0\0sig_OneToZero\0"
    "slot_IOInit\0slot_IOJerkOn\0slot_IOJerkOff"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IOLabel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IOLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IOLabel *_t = static_cast<IOLabel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_ZeroToOne(); break;
        case 1: _t->sig_OneToZero(); break;
        case 2: _t->slot_IOInit(); break;
        case 3: _t->slot_IOJerkOn(); break;
        case 4: _t->slot_IOJerkOff(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IOLabel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IOLabel::sig_ZeroToOne)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (IOLabel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IOLabel::sig_OneToZero)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject IOLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_IOLabel.data,
      qt_meta_data_IOLabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IOLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IOLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IOLabel.stringdata0))
        return static_cast<void*>(const_cast< IOLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int IOLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void IOLabel::sig_ZeroToOne()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void IOLabel::sig_OneToZero()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
