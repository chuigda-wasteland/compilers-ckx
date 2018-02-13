#include "frontend/lex/ckx_token_stream.hpp"
#include "frontend/parse/ckx_parser.hpp"
#include "frontend/sema/ckx_sema.hpp"
#include "saber/string_pool.hpp"

saber_string str =
R"cc(

fn print(vi64 number): void;
fn print(vr64 number): void;
fn print(vu64 number): void;

fn i2u(vi64 number): vu64 {
    return static_cast<vu64>(number);
}

fn i2f(vi64 number): vr64 {
    return static_cast<vr64>(number);
}

fn main(): vi32 {
    vi64 number = 65536;
    print(number);
    print(i2u(number));
    print(i2f(number));
    return 0;
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
