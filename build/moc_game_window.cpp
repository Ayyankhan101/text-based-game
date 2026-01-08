/****************************************************************************
** Meta object code from reading C++ file 'game_window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/game_window.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
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
struct qt_meta_tag_ZN7StatBarE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN7StatBarE = QtMocHelpers::stringData(
    "StatBar",
    "value"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN7StatBarE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags, notifyId, revision
       1, QMetaType::Int, 0x00015103, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject StatBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN7StatBarE.offsetsAndSizes,
    qt_meta_data_ZN7StatBarE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN7StatBarE_t,
        // property 'value'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<StatBar, std::true_type>
    >,
    nullptr
} };

void StatBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StatBar *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->value(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setValue(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *StatBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StatBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN7StatBarE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int StatBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN10GameWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10GameWindowE = QtMocHelpers::stringData(
    "GameWindow",
    "gameClosed",
    "",
    "returnToMenu",
    "onChoiceA",
    "onChoiceB",
    "onUndo",
    "onSave",
    "onLoad",
    "onInventory",
    "onPack",
    "updateStats",
    "onNewGame",
    "onAbout",
    "onExit",
    "onReturnToMenu"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10GameWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x06,    1 /* Public */,
       3,    0,   99,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,  100,    2, 0x08,    3 /* Private */,
       5,    0,  101,    2, 0x08,    4 /* Private */,
       6,    0,  102,    2, 0x08,    5 /* Private */,
       7,    0,  103,    2, 0x08,    6 /* Private */,
       8,    0,  104,    2, 0x08,    7 /* Private */,
       9,    0,  105,    2, 0x08,    8 /* Private */,
      10,    0,  106,    2, 0x08,    9 /* Private */,
      11,    0,  107,    2, 0x08,   10 /* Private */,
      12,    0,  108,    2, 0x08,   11 /* Private */,
      13,    0,  109,    2, 0x08,   12 /* Private */,
      14,    0,  110,    2, 0x08,   13 /* Private */,
      15,    0,  111,    2, 0x08,   14 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject GameWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10GameWindowE.offsetsAndSizes,
    qt_meta_data_ZN10GameWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10GameWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GameWindow, std::true_type>,
        // method 'gameClosed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'returnToMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onChoiceA'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onChoiceB'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUndo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSave'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLoad'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onInventory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAbout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReturnToMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GameWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->gameClosed(); break;
        case 1: _t->returnToMenu(); break;
        case 2: _t->onChoiceA(); break;
        case 3: _t->onChoiceB(); break;
        case 4: _t->onUndo(); break;
        case 5: _t->onSave(); break;
        case 6: _t->onLoad(); break;
        case 7: _t->onInventory(); break;
        case 8: _t->onPack(); break;
        case 9: _t->updateStats(); break;
        case 10: _t->onNewGame(); break;
        case 11: _t->onAbout(); break;
        case 12: _t->onExit(); break;
        case 13: _t->onReturnToMenu(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (GameWindow::*)();
            if (_q_method_type _q_method = &GameWindow::gameClosed; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (GameWindow::*)();
            if (_q_method_type _q_method = &GameWindow::returnToMenu; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *GameWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10GameWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int GameWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void GameWindow::gameClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GameWindow::returnToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
