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

template <typename CkxTokenStream>
class ckx_parser_impl_test final :
        public detail::ckx_parser_impl<CkxTokenStream>
{
    using base = detail::ckx_parser_impl<CkxTokenStream>;

public:
    void test();

private:
    void initialize_test();
    void cleanup_test();
};



int main()
{
    ckx_parser_impl_test<ckx_default_token_stream> test;
    test.test();
    return 0;
}



template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test()
{
    ckx_fp_writer writer { stdout };

    {
        ckx_test_filereader reader {"vi8 a, b;"};
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        ckx_ast_decl_stmt *decl = base::parse_decl_stmt();
        decl->ast_dump(writer, 0);
        delete decl;

        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }

    {
        ckx_test_filereader reader {"vi8 * const array;"};
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        ckx_ast_decl_stmt *decl = base::parse_decl_stmt();
        decl->ast_dump(writer, 0);
        delete decl;

        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }

    {
        ckx_test_filereader reader {"vr32 a = 5+2, b = a*3+2;"};
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        ckx_ast_decl_stmt *decl = base::parse_decl_stmt();
        decl->ast_dump(writer, 0);
        delete decl;

        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }

    {
        ckx_test_filereader reader {"student const * pcstudent;"};
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        base::typename_table->add_typename(
            saber_string_pool::create_view("student"));
        ckx_ast_decl_stmt *decl = base::parse_decl_stmt();
        decl->ast_dump(writer, 0);
        delete decl;

        assert(base::error_list->empty());
        assert(base::warn_list->empty());
        cleanup_test();
    }
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::initialize_test()
{
    base::error_list = new saber::list<ckx_error>;
    base::warn_list = new saber::list<ckx_error>;
    base::typename_table = new detail::ckx_typename_table;
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::cleanup_test()
{
    delete base::error_list;
    delete base::warn_list;
    delete base::typename_table;
}
