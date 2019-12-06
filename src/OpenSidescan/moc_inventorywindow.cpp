/****************************************************************************
** Meta object code from reading C++ file 'inventorywindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "inventorywindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inventorywindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InventoryWindow_t {
    QByteArrayData data[9];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InventoryWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InventoryWindow_t qt_meta_stringdata_InventoryWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "InventoryWindow"
QT_MOC_LITERAL(1, 16, 14), // "objectSelected"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "GeoreferencedObject*"
QT_MOC_LITERAL(4, 53, 6), // "object"
QT_MOC_LITERAL(5, 60, 21), // "refreshInventoryTable"
QT_MOC_LITERAL(6, 82, 13), // "doubleClicked"
QT_MOC_LITERAL(7, 96, 11), // "QModelIndex"
QT_MOC_LITERAL(8, 108, 5) // "index"

    },
    "InventoryWindow\0objectSelected\0\0"
    "GeoreferencedObject*\0object\0"
    "refreshInventoryTable\0doubleClicked\0"
    "QModelIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InventoryWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   32,    2, 0x0a /* Public */,
       6,    1,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void InventoryWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InventoryWindow *_t = static_cast<InventoryWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->objectSelected((*reinterpret_cast< GeoreferencedObject*(*)>(_a[1]))); break;
        case 1: _t->refreshInventoryTable(); break;
        case 2: _t->doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InventoryWindow::*)(GeoreferencedObject * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InventoryWindow::objectSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InventoryWindow::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_InventoryWindow.data,
      qt_meta_data_InventoryWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *InventoryWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InventoryWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InventoryWindow.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int InventoryWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void InventoryWindow::objectSelected(GeoreferencedObject * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
