TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ckx_token.cpp \
    ../saber/src/safe_stl_general.cpp \
    ckx_token_stream.cpp \
    ckx_parser.cpp \
    ckx_ast_node.cpp \
    ckx_type.cpp \
    ckx_env_table.cpp \
    ckx_token_set.cpp \
    ckx_ast_dump.cpp \
    ckx_type_dump.cpp \
    ckx_operator.cpp \
    ../shared/src/file_reader.cpp \
    ../shared/src/file_writer.cpp \
    ckx_prelexed_type.cpp

HEADERS += \
    we_file_reader.hpp \
    ckx_token.hpp \
    ../saber/include/algorithm.hpp \
    ../saber/include/deque.hpp \
    ../saber/include/excalibur.hpp \
    ../saber/include/forward_list.hpp \
    ../saber/include/free_tree.hpp \
    ../saber/include/functional.hpp \
    ../saber/include/iterator.hpp \
    ../saber/include/list.hpp \
    ../saber/include/map.hpp \
    ../saber/include/memory.hpp \
    ../saber/include/saber_traits.hpp \
    ../saber/include/safe_stl_general.hpp \
    ../saber/include/set.hpp \
    ../saber/include/strings.defs.h \
    ../saber/include/type_traits.hpp \
    ../saber/include/utility.hpp \
    ../saber/include/object_pool.hpp \
    ../saber/include/vector.hpp \
    ../saber/include/string.hpp \
    ../saber/include/result.hpp \
    ../saber/include/unordered_map.hpp \
    ../shared/include/file_reader.hpp \
    ../shared/include/file_writer.hpp \
    ../shared/include/defs.hpp \
    ckx_token_stream.hpp \
    ckx_parser.hpp \
    ckx_ast_node.hpp \
    ckx_error.hpp \
    ckx_type.hpp \
    ckx_env_table.hpp \
    gg.h \
    ckx_ast_node_fwd.hpp \
    ../saber/include/unordered_set.hpp \
    ckx_token_set.hpp \
    ../saber/include/string_pool.hpp \
    ../saber/include/string_pool.hpp \
    we::we_file_writer.hpp \
    ckx_operator.hpp \
    ckx_parser_impl.hpp \
    ckx_prelexed_type.hpp \
    ckx_sema.hpp \
    ckx_sema_result.hpp \
    ckx_sema_result_fwd.hpp

INCLUDEPATH += ../saber/include/
INCLUDEPATH += ../shared/include/

DISTFILES += \
    LICENSE \
    production.txt \
    ../README.md
