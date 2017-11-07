TEMPLATE = subdirs

SUBDIRS += \
    compiler \
    test/test_parse_expr \
    test/test_parse_decl \
    test/test_parse_func \
    test/test_parse_udt \
    test/test_parse_stmt \
    test/test_parser \
    llvm_ir_builder \