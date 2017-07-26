TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ckx_file_reader.cpp \
    ckx_token.cpp \
    saber/src/safe_stl_general.cpp \
    ckx_token_stream.cpp \
    ckx_parser.cpp \
    ckx_ast_node.cpp

HEADERS += \
    ckx_file_reader.hpp \
    ckx_token.hpp \
    saber/include/algorithm.hpp \
    saber/include/deque.hpp \
    saber/include/excalibur.hpp \
    saber/include/forward_list.hpp \
    saber/include/free_tree.hpp \
    saber/include/functional.hpp \
    saber/include/iterator.hpp \
    saber/include/list.hpp \
    saber/include/map.hpp \
    saber/include/memory.hpp \
    saber/include/saber_traits.hpp \
    saber/include/safe_stl_general.hpp \
    saber/include/set.hpp \
    saber/include/strings.defs.h \
    saber/include/type_traits.hpp \
    saber/include/utility.hpp \
    saber/include/vector.hpp \
    saber/include/string.hpp \
    defs.hpp \
    ckx_token_stream.hpp \
    ckx_parser.hpp \
    ckx_ast_node.hpp

INCLUDEPATH += saber/include/

DISTFILES += \
    LICENSE
