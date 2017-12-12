TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../../compiler/ckx_type.cpp \
           ../../saber/src/safe_stl_general.cpp \
    test.cpp

HEADERS += ../../compiler/ckx_type.hpp

INCLUDEPATH += ../../saber/include/
INCLUDEPATH += ../../shared/include/
INCLUDEPATH += ../../compiler/
