# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = compilers-ckx

HEADERS = \
   $$PWD/include/frontend/AST/ckx_ast_node.hpp \
   $$PWD/include/frontend/AST/ckx_ast_node_fwd.hpp \
   $$PWD/include/frontend/AST/ckx_operator.hpp \
   $$PWD/include/frontend/lex/ckx_token.hpp \
   $$PWD/include/frontend/lex/ckx_token_stream.hpp \
   $$PWD/include/frontend/parse/ckx_parser.hpp \
   $$PWD/include/frontend/parse/ckx_parser_impl.hpp \
   $$PWD/include/frontend/parse/ckx_prelexed_type.hpp \
   $$PWD/include/frontend/parse/ckx_token_set.hpp \
   $$PWD/include/frontend/parse/gg.h \
   $$PWD/include/frontend/sema/ckx_env_table.hpp \
   $$PWD/include/frontend/sema/ckx_func_name_mangler.hpp \
   $$PWD/include/frontend/sema/ckx_sema.hpp \
   $$PWD/include/frontend/sema/ckx_sema_fwd.hpp \
   $$PWD/include/frontend/sema/ckx_sema_result.hpp \
   $$PWD/include/frontend/sema/ckx_sema_result_fwd.hpp \
   $$PWD/include/frontend/sema/ckx_type.hpp \
   $$PWD/include/frontend/util/ckx_error.hpp \
   $$PWD/include/llvm/llvm_implicit_list.hpp \
   $$PWD/include/llvm/llvm_inst.hpp \
   $$PWD/include/llvm/llvm_inst_detail.hpp \
   $$PWD/include/llvm/llvm_ir_builder.hpp \
   $$PWD/include/llvm/llvm_ir_builder_impl.hpp \
   $$PWD/include/llvm/llvm_type.hpp \
   $$PWD/include/llvm/llvm_value.hpp \
   $$PWD/include/llvm/opdef.hpp \
   $$PWD/include/llvm/llvm_value_detail.hpp \
   $$PWD/include/saber/algorithm.hpp \
   $$PWD/include/saber/c8assert.hpp \
   $$PWD/include/saber/excalibur.hpp \
   $$PWD/include/saber/functional.hpp \
   $$PWD/include/saber/iterator.hpp \
   $$PWD/include/saber/list.hpp \
   $$PWD/include/saber/memory.hpp \
   $$PWD/include/saber/object_pool.hpp \
   $$PWD/include/saber/optional.hpp \
   $$PWD/include/saber/result.hpp \
   $$PWD/include/saber/saber_traits.hpp \
   $$PWD/include/saber/safe_stl_general.hpp \
   $$PWD/include/saber/string.hpp \
   $$PWD/include/saber/string_pool.hpp \
   $$PWD/include/saber/strings.defs.h \
   $$PWD/include/saber/type_traits.hpp \
   $$PWD/include/saber/unordered_map.hpp \
   $$PWD/include/saber/unordered_set.hpp \
   $$PWD/include/saber/utility.hpp \
   $$PWD/include/saber/vector.hpp \
   $$PWD/include/we/defs.hpp \
   $$PWD/include/we/file_reader.hpp \
   $$PWD/include/we/file_writer.hpp

SOURCES = \
   $$PWD/src/driver/main.cpp \
   $$PWD/src/frontend/AST/ckx_ast_dump.cpp \
   $$PWD/src/frontend/AST/ckx_ast_node.cpp \
   $$PWD/src/frontend/lex/ckx_operator.cpp \
   $$PWD/src/frontend/lex/ckx_token.cpp \
   $$PWD/src/frontend/lex/ckx_token_stream.cpp \
   $$PWD/src/frontend/parse/ckx_parser.cpp \
   $$PWD/src/frontend/parse/ckx_prelexed_type.cpp \
   $$PWD/src/frontend/parse/ckx_token_set.cpp \
   $$PWD/src/frontend/sema/ckx_env_table.cpp \
   $$PWD/src/frontend/sema/ckx_func_name_mangler.cpp \
   $$PWD/src/frontend/sema/ckx_sema.cpp \
   $$PWD/src/frontend/sema/ckx_translate.cpp \
   $$PWD/src/frontend/sema/ckx_type.cpp \
   $$PWD/src/frontend/sema/ckx_type_dump.cpp \
   $$PWD/src/llvm/llvm_inst.cpp \
   $$PWD/src/llvm/llvm_ir_builder.cpp \
   $$PWD/src/llvm/llvm_ir_builder_impl.cpp \
   $$PWD/src/llvm/llvm_value.cpp \
   $$PWD/src/saber/safe_stl_general.cpp \
   $$PWD/src/we/file_reader.cpp \
   $$PWD/src/we/file_writer.cpp

INCLUDEPATH = include

#DEFINES = 

