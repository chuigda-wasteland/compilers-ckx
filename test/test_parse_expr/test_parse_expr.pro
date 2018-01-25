
CONFIG += c++14 console
CONFIG -= app_bundle
CONFIG -= qt

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../saber/src/safe_stl_general.cpp \
    ../../shared/src/file_writer.cpp \
    ../../shared/src/file_reader.cpp \
    ../../compiler/ckx_type.cpp \
    ../../compiler/ckx_type_dump.cpp \
    ../../compiler/ckx_ast_dump.cpp \
    ../../compiler/ckx_ast_node.cpp \
    ../../compiler/ckx_env_table.cpp \
    ../../compiler/ckx_operator.cpp \
    ../../compiler/ckx_parser.cpp \
    ../../compiler/ckx_token.cpp \
    ../../compiler/ckx_token_set.cpp \
    ../../compiler/ckx_token_stream.cpp \
    ../../compiler/ckx_translate.cpp \
    ../../compiler/ckx_func_name_mangler.cpp\
    test_expr.cpp

INCLUDEPATH += \
    ../../saber/include/ \
    ../../shared/include/ \
    ../../compiler/
