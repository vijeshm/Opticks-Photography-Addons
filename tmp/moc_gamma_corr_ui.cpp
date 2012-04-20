/****************************************************************************
** Meta object code from reading C++ file 'gamma_corr_ui.h'
**
** Created: Fri 20. Apr 17:04:18 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../New Plugins/gamma_corr_ui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamma_corr_ui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_gamma_corr_ui[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   15,   14,   14, 0x08,
      39,   14,   14,   14, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_gamma_corr_ui[] = {
    "gamma_corr_ui\0\0t\0setGammaValue(double)\0"
    "setGammaValue()\0"
};

const QMetaObject gamma_corr_ui::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_gamma_corr_ui,
      qt_meta_data_gamma_corr_ui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gamma_corr_ui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gamma_corr_ui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gamma_corr_ui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gamma_corr_ui))
        return static_cast<void*>(const_cast< gamma_corr_ui*>(this));
    return QDialog::qt_metacast(_clname);
}

int gamma_corr_ui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setGammaValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: setGammaValue(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
