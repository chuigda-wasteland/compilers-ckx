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
    void test_parse_basic_expr();
    void test_parse_type();
    void test_parse_postfix_expr();
    void test_parse_unary_expr();
    void test_parse_cast_expr();
    void test_parse_binary_expr();
    void test_parse_cond_expr();
    void test_parse_assign_expr();

private:
    void initialize_test();
    void cleanup_test();
};



int main()
{
    ckx_parser_impl_test<ckx_default_token_stream> test;
    test.test_parse_basic_expr();
    test.test_parse_type();
    test.test_parse_postfix_expr();
    test.test_parse_unary_expr();
    test.test_parse_cast_expr();
    test.test_parse_binary_expr();
    test.test_parse_cond_expr();
    test.test_parse_assign_expr();
    return 0;
}



template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test_parse_basic_expr()
{
    ckx_test_filereader reader {"123 literal number   39.5 array"};
    ckx_fp_writer writer { stdout };

    base::token_stream = new CkxTokenStream(reader);
    initialize_test();
    for (int i = 0; i < 5; i++)
    {
        ckx_ast_expr *expr =
            base::parse_basic_expr();
        expr->ast_dump(writer, 0);
        delete expr;
    }
    cleanup_test();
    base::token_stream = nullptr;
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test_parse_type()
{
    ckx_fp_writer writer { stdout };

    {
        ckx_test_filereader reader { "vi8 const * const * const" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        saber_ptr<ckx_type> type = base::parse_type();
        writer.write(type->to_string());
        writer.write("\n");

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "vi8 *" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        saber_ptr<ckx_type> type = base::parse_type();
        writer.write(type->to_string());
        writer.write("\n");

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "vr32 const" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        saber_ptr<ckx_type> type = base::parse_type();
        writer.write(type->to_string());
        writer.write("\n");

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "st" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        ckx_struct_type *struct_type =
            new ckx_struct_type(saber_string_pool::get().create_view("st"));
        struct_type->add_field(
            saber_string_pool::get().create_view("integer"),
            ckx_type_helper::get_type(ckx_token::type::tk_vi64));
        struct_type->add_field(
            saber_string_pool::get().create_view("real"),
            ckx_type_helper::get_type(ckx_token::type::tk_vr64));
        struct_type->add_field(
            saber_string_pool::get().create_view("nquadra"),
            ckx_type_helper::pointer_to(
                ckx_type_helper::get_type(ckx_token::type::tk_vi64)));
        saber_ptr<ckx_type> saber_type{ struct_type };
        base::env()->add_new_type(
            saber_string_pool::get().create_view("st"), saber_type);

        saber_ptr<ckx_type> type = base::parse_type();
        writer.write(type->to_string());
        writer.write("\n");

        cleanup_test();
        base::token_stream = nullptr;
    }
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test_parse_postfix_expr()
{
    ckx_fp_writer writer { stdout };

    {
        ckx_test_filereader reader { "a[5]" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        base::env()->add_new_var(
            saber_string_pool::get().create_view("a"),
            ckx_type_helper::pointer_to(
                ckx_type_helper::get_type(ckx_token::type::tk_vi8)));

        ckx_ast_expr* expr = base::parse_postfix_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "sum(a, 2, sum(5, 6), 4, number)" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();
        base::env()->add_new_var(
            saber_string_pool::get().create_view("sum"),
            ckx_type_helper::get_type(ckx_token::type::tk_vu8));
        base::env()->add_new_var(
            saber_string_pool::get().create_view("a"),
            ckx_type_helper::get_type(ckx_token::type::tk_vu8));
        base::env()->add_new_var(
            saber_string_pool::get().create_view("b"),
            ckx_type_helper::get_type(ckx_token::type::tk_vu8));

        ckx_ast_expr* expr = base::parse_postfix_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test_parse_unary_expr()
{
    ckx_fp_writer writer { stdout };

    {
        ckx_test_filereader reader { "++number" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "--number" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "--++number" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "*++array" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "&integer" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "~literal" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "!number" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr* expr = base::parse_unary_expr();
        expr->ast_dump(writer, 0);
        delete expr;
        cleanup_test();
        base::token_stream = nullptr;
    }
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test_parse_cast_expr()
{
    ckx_fp_writer writer { stdout };
    {
        ckx_test_filereader reader { "static_cast<vi8>(number)" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_cast_expr();
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "reinterpret_cast<vr32*>(array)" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_cast_expr();
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader {
            "const_cast<vr32 const*>(reinterpret_cast<vr32*>(array))" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_cast_expr();
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }
}

template<typename CkxTokenStream>
void ckx_parser_impl_test<CkxTokenStream>::test_parse_binary_expr()
{
    ckx_fp_writer writer { stdout };

    {
        ckx_test_filereader reader { "number * integer" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_binary_expr(0);
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader { "number * *array + integer" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_binary_expr(0);
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader {
            "number>integer && integer>literal || integer+literal>*array" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_binary_expr(0);
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }
}

template<typename CkxTokenStream>
void ckx_parser_impl_test<CkxTokenStream>::test_parse_cond_expr()
{
    ckx_fp_writer writer { stdout };
    {
        ckx_test_filereader reader { "number>integer ? number : integer" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_cond_expr();
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }
}

template<typename CkxTokenStream>
void ckx_parser_impl_test<CkxTokenStream>::test_parse_assign_expr()
{
    ckx_fp_writer writer { stdout };
    {
        ckx_test_filereader reader { "number = literal + integer - *array" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_assign_expr();
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }

    {
        ckx_test_filereader reader {
            "(number>literal?number:literal) = *array" };
        base::token_stream = new CkxTokenStream(reader);
        initialize_test();

        ckx_ast_expr *expr = base::parse_assign_expr();
        expr->ast_dump(writer, 0);

        cleanup_test();
        base::token_stream = nullptr;
    }
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::initialize_test()
{
    base::enter_scope(new ckx_env(nullptr));
    base::error_list = new saber::list<ckx_error*>;
    base::warn_list = new saber::list<ckx_error*>;

    std::array<saber_string_view, 3> names
    {
        /// Clang recommended me to add a pair of brace here,
        /// but I don't know why. >_<
        {
            saber_string_pool::get().create_view("number"),
            saber_string_pool::get().create_view("integer"),
            saber_string_pool::get().create_view("literal")
        }
    };

    for (auto& name : names)
        base::env()->add_new_var(
            name, ckx_type_helper::get_type(ckx_token::type::tk_vi32));

    base::env()->add_new_var(
        saber_string_pool::get().create_view("array"),
        ckx_type_helper::pointer_to(
            ckx_type_helper::get_type(ckx_token::type::tk_vi8)));
}

template <typename CkxTokenStream>
void ckx_parser_impl_test<CkxTokenStream>::cleanup_test()
{
    delete base::current_env;
    delete base::error_list;
    delete base::warn_list;
    base::current_env = nullptr;
}
