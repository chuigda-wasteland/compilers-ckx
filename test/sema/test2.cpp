#include "frontend/lex/ckx_token_stream.hpp"
#include "frontend/parse/ckx_parser.hpp"
#include "frontend/sema/ckx_sema.hpp"
#include "saber/string_pool.hpp"

saber_string str =
R"cc(

struct s { vi8 a; vi16 b, c; s* d; }

fn one(): vi32 {
    return 1;
}

fn foo(vi8 a, vi8 b): vi8;
fn bar(vi16 a, vi8* const b): void;
fn baz(s s1): void;

fn main(vi8 argc, vi8** argv): vi32 {
    vi8 c;
    vi32 a = 5, b = 65536;
    s s1;
    return 65536;
}

)cc";

int main()
{
    using namespace ckx;
    using namespace we;

    we_test_filereader reader1(saber::move(str));
    ckx_token_stream stream1(reader1);

    ckx_parser parser1;
    ckx_parser::parse_result parse_result = parser1.parse(&stream1);

    ckx_sema_engine engine;
    parse_result.trans_unit->accept(engine);

    we_fp_writer writer(stdout);
    engine.test_print(writer);
}
