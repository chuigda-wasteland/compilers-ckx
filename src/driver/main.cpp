#include "frontend/lex/ckx_token_stream.hpp"
#include "frontend/parse/ckx_parser.hpp"
#include "frontend/sema/ckx_sema.hpp"

int main(int argc, char** argv)
{
    using namespace ckx;
    using namespace we;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ckxc filename\n");
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == nullptr)
    {
        fprintf(stderr, "File %s not found\n", argv[1]);
        return -2;
    }

    we_fp_reader reader(fp);
    ckx_token_stream stream(reader);
    ckx_sema_engine sema;

    ckx_parser().parse(&stream).trans_unit->accept(sema);
    we_fp_writer writer(stdout);
    sema.test_print(writer);
    fclose(fp);

    return 0;
}
