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
public:
    void test_parse_basic_expr();
    void test_parse_unary_expr();

private:
    void initialize_test();
    void cleanup_test();
};



int main()
{
    ckx_parser_impl_test<ckx_default_token_stream> test;
    test.test_parse_basic_expr();
    return 0;
}



template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::test_parse_basic_expr()
{
    ckx_test_filereader reader {"123 literal number   39.5 array"};
    ckx_fp_writer writer { stdout };
    detail::ckx_parser_impl<CkxTokenStream>::token_stream =
                new ckx_default_token_stream(reader);
    initialize_test();

    for (int i = 0; i < 5; i++)
    {
        ckx_ast_expr *expr =
            detail::ckx_parser_impl<CkxTokenStream>::parse_basic_expr();
        expr->ast_dump(writer, 0);
        delete expr;
    }

    cleanup_test();
}

template <typename CkxTokenStream>
void
ckx_parser_impl_test<CkxTokenStream>::initialize_test()
{
    detail::ckx_parser_impl<CkxTokenStream>::current_env =
            new ckx_env(nullptr);
    detail::ckx_parser_impl<CkxTokenStream>::error_list =
            new saber::list<ckx_error*>;
    detail::ckx_parser_impl<CkxTokenStream>::warn_list =
            new saber::list<ckx_error*>;

    std::array<saber_string_view, 3> names
    {
        saber_string_pool::get().create_view("number"),
        saber_string_pool::get().create_view("integer"),
        saber_string_pool::get().create_view("literal"),
    };

    for (auto& name : names)
        detail::ckx_parser_impl<CkxTokenStream>::env()->add_new_var(
            name, ckx_type_helper::get_type(ckx_token::type::tk_vi32));

    detail::ckx_parser_impl<CkxTokenStream>::env()->add_new_var(
        saber_string_pool::get().create_view("array"),
        ckx_type_helper::pointer_to(
            ckx_type_helper::get_type(ckx_token::type::tk_vi8)));
}

template <typename CkxTokenStream>
void ckx_parser_impl_test<CkxTokenStream>::cleanup_test()
{
    delete detail::ckx_parser_impl<CkxTokenStream>::current_env;
    delete detail::ckx_parser_impl<CkxTokenStream>::error_list;
    delete detail::ckx_parser_impl<CkxTokenStream>::warn_list;
}
