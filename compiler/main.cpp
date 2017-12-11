#include "ckx_token_stream.hpp"
#include "ckx_fake_parser.h"
#include "ckx_error_intepreter.h"
#include "file_reader.hpp"

#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[])
{
    using namespace ckx;

    if (argc != 2)
    {
        printf("Usage: ckx [filename]\n");
        system("pause");
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Bad filename\n");
        system("pause");
        return -1;
    }
    we::we_fp_reader reader(fp);
    ckx_token_stream stream(reader);

    ckx_fake_parser parser;
    saber::vector<ckx_error> errors = parser.make_errors(stream);
    fclose(fp);

    fp = fopen(argv[1], "r");
    we::we_fp_reader reader1(fp);
    ckx_error_intepreter intepreter =
        ckx_error_intepreter(reader1);
    fclose(fp);

    for (auto &error : errors)
        intepreter.interpret(error);
    system("pause");
    return 0;
}
