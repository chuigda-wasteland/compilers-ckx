CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    llvm_value.cpp \
    ../saber/src/safe_stl_general.cpp \
    llvm_inst.cpp \
    llvm_ir_builder.cpp

HEADERS += \
    llvm_value.hpp \
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
    ../saber/include/vector.hpp \
    ../saber/include/string.hpp \
    ../saber/include/unordered_map.hpp \
    llvm_value_detail.hpp \
    llvm_inst.hpp \
    llvm_inst_detail.hpp \
    llvm_type.hpp \
    llvm_implicit_list.hpp \
    llvm_ir_builder.hpp \
    opdef.hpp \
    llvm_ir_builder_impl.hpp

INCLUDEPATH +=\
    ../saber/include/
