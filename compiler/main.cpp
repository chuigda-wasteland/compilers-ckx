#include "ckx_token_stream.hpp"
#include "ckx_parser.hpp"

int main()
{
    using namespace ckx;

    ckx_parser<ckx_default_token_stream> parser;
    parser.parse(nullptr);

    return 0;
}
