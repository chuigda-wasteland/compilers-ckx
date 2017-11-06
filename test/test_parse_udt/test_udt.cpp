/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

/// @note This file contains a MANUAL test. Debuggers should read the log
/// themselves and manually detect bugs.

#include "ckx_parser_impl.hpp"
#include "memory.hpp"

using namespace ckx;

class ckx_test_filereader final implements ckx_file_reader
{
public:
    ckx_test_filereader(saber_string&& _str) : str(saber::move(_str)) {}
    ~ckx_test_filereader() = default;
    qchar get_next_char(void) override final
    {
        pos++;
        if (pos-1 < str.size()) return str[pos-1];
        else return static_cast<qchar>(saber::char_traits<qchar>::eof());
    }

private:
    qsizet pos = 0;
    saber_string str;
};


class ckx_parser_impl_test final :
        public detail::ckx_parser_impl
{
    using base = detail::ckx_parser_impl;

public:
    void test_parse_struct();
    /// @note we are not about to deal with variant type
    /// since the processing of variant is identical with struct
    /// void test_parse_variant();
    void test_parse_enum();
    void test_parse_alias();

private:
    void initialize_test();
    void cleanup_test();
};



int main()
{
    ckx_parser_impl_test test;
    test.test_parse_struct();
    test.test_parse_enum();
    test.test_parse_alias();

    return 0;
}



void
ckx_parser_impl_test::test_parse_struct()
{
    ckx_fp_writer writer { stdout };

    {
        const char* str =
R"noip(
        struct student {
            vi8 const* name;
            vu8 chinese;
            vu8 math;
            vu8 english;
        }
)noip";

        ckx_test_filereader reader { str };
        base::token_stream = new ckx_token_stream(reader);
        initialize_test();
        ckx_ast_struct_stmt *stmt =
            base::template parse_record_stmt<ckx_ast_struct_stmt>();
        stmt->ast_dump(writer, 0);
        delete stmt;
        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }

    {
        const char* str =
R"noip(
        struct listNode {
            vi8 * const data;
            listNode * next;
        }
)noip";

        ckx_test_filereader reader { str };
        base::token_stream = new ckx_token_stream(reader);
        initialize_test();
        ckx_ast_struct_stmt *stmt =
                base::template parse_record_stmt<ckx_ast_struct_stmt>();
        stmt->ast_dump(writer, 0);
        delete stmt;
        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }
}


void
ckx_parser_impl_test::test_parse_enum()
{
    ckx_fp_writer writer { stdout };

    {
        const char* str =
R"noip(
        enum color {
            red = 1,
            yellow = 2,
            blue = 3
        }
)noip";
        ckx_test_filereader reader { str };
        base::token_stream = new ckx_token_stream(reader);
        initialize_test();
        ckx_ast_enum_stmt *stmt = base::parse_enum_stmt();
        stmt->ast_dump(writer, 0);
        delete stmt;

        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }
}


void
ckx_parser_impl_test::test_parse_alias()
{
    ckx_fp_writer writer { stdout };

    {
        ckx_test_filereader reader { "alias pcvi8 = vi8 const *;" };
        base::token_stream = new ckx_token_stream(reader);
        initialize_test();
        ckx_ast_alias_stmt *stmt = base::parse_alias_stmt();
        stmt->ast_dump(writer, 0);
        delete stmt;
        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }
}


void
ckx_parser_impl_test::initialize_test()
{
    base::error_list = new saber::list<ckx_error>;
    base::warn_list = new saber::list<ckx_error>;
    base::typename_table = new detail::ckx_typename_table;
}


void
ckx_parser_impl_test::cleanup_test()
{
    delete base::error_list;
    delete base::warn_list;
    delete base::typename_table;
}
