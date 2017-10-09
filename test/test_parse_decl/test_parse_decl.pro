TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../saber/src/safe_stl_general.cpp \
    ../../compiler/ckx_type.cpp \
    ../../compiler/ckx_ast_dump.cpp \
    ../../compiler/ckx_ast_node.cpp \
    ../../compiler/ckx_env_table.cpp \
    ../../compiler/ckx_file_reader.cpp \
    ../../compiler/ckx_file_writer.cpp \
    ../../compiler/ckx_operator.cpp \
    ../../compiler/ckx_parser.cpp \
    ../../compiler/ckx_token.cpp \
    ../../compiler/ckx_token_set.cpp \
    ../../compiler/ckx_token_stream.cpp

INCLUDEPATH += \
    ../../saber/include/ \
    ../../compiler/

SOURCES += \
    test_decl.cpp
