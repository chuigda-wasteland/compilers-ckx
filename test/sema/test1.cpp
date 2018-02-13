#include "frontend/lex/ckx_token_stream.hpp"
#include "frontend/parse/ckx_parser.hpp"
#include "frontend/sema/ckx_sema.hpp"
#include "saber/string_pool.hpp"

saber_string str =
R"cc(

fn heap_alloc(vu64 size): void*;
fn heap_free(void* ptr): void;

fn input(vi8* ptr): void;
fn print(vi8 number): void;

fn main(): vi32 {
    vi8 *array = reinterpret_cast<vi8*>(heap_alloc(4));
    input(array);
    input(array + 1);
    input(array + 2);
    *(array+3) = *(array+1) * *(array+2) + *(array);
    print(*array+3);
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
