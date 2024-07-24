/****************************************************************************
** Meta object code from reading C++ file 'SettingWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Client_new/SettingWindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SettingWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSettingWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSettingWindowENDCLASS = QtMocHelpers::stringData(
    "SettingWindow",
    "logoutUser",
    "",
    "do_friendListClosed",
    "pbtn_returnClicked",
    "pbtn_saveClicked",
    "pbtn_logoutClicked",
    "do_settingAffair",
    "json"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSettingWindowENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[14];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[19];
    char stringdata5[17];
    char stringdata6[19];
    char stringdata7[17];
    char stringdata8[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSettingWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSettingWindowENDCLASS_t qt_meta_stringdata_CLASSSettingWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "SettingWindow"
        QT_MOC_LITERAL(14, 10),  // "logoutUser"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 19),  // "do_friendListClosed"
        QT_MOC_LITERAL(46, 18),  // "pbtn_returnClicked"
        QT_MOC_LITERAL(65, 16),  // "pbtn_saveClicked"
        QT_MOC_LITERAL(82, 18),  // "pbtn_logoutClicked"
        QT_MOC_LITERAL(101, 16),  // "do_settingAffair"
        QT_MOC_LITERAL(118, 4)   // "json"
    },
    "SettingWindow",
    "logoutUser",
    "",
    "do_friendListClosed",
    "pbtn_returnClicked",
    "pbtn_saveClicked",
    "pbtn_logoutClicked",
    "do_settingAffair",
    "json"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSettingWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   51,    2, 0x0a,    2 /* Public */,
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    0,   54,    2, 0x08,    5 /* Private */,
       7,    1,   55,    2, 0x08,    6 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject SettingWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSettingWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSettingWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSettingWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SettingWindow, std::true_type>,
        // method 'logoutUser'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'do_friendListClosed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pbtn_returnClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pbtn_saveClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pbtn_logoutClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'do_settingAffair'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>
    >,
    nullptr
} };

void SettingWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->logoutUser(); break;
        case 1: _t->do_friendListClosed(); break;
        case 2: _t->pbtn_returnClicked(); break;
        case 3: _t->pbtn_saveClicked(); break;
        case 4: _t->pbtn_logoutClicked(); break;
        case 5: _t->do_settingAffair((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SettingWindow::*)();
            if (_t _q_method = &SettingWindow::logoutUser; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *SettingWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSettingWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SettingWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SettingWindow::logoutUser()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
