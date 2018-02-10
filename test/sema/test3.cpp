#include "frontend/lex/ckx_token_stream.hpp"
#include "frontend/parse/ckx_parser.hpp"
#include "frontend/sema/ckx_sema.hpp"
#include "saber/string_pool.hpp"

saber_string str =
R"cc(

fn add(vr64 a, vr64 b): vr64;
fn add(vi64 a, vi64 b): vi64;
fn print(vr64 number): vr64;
fn print(vi64 number): vi64;
fn print(vi8 number): vi8;
fn input(vi32 *p): vi32;
fn input(vr64 *p): vr64;

fn heap_alloc(): vi8*;

fn main(): vi32 {
    vr64 a, b;
    input(&a);
    input(&b);
    print(add(a, b));
    vi32 c, d;
    input(&c);
    input(&d);
    print(add(c, d));
    vi8* e = heap_alloc();
    (*e) = 5;
    print(*e);
    print(*heap_alloc());
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
