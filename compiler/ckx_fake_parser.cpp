#include "ckx_fake_parser.h"
#include <cstdlib>
#include <ctime>

namespace ckx
{

static const char* prompts[]
{
    "Unexpected token, expected ChenKX.",
    "Cannot sentialize given part since I Love ChenKX.",
    "I don't know what's wrong, just love CKX.",
    "Try replacing the underlined token, with 'ckx'.",
    "Unknown token in the scope. try adding a 'ckx' to repair.",
    "Warning: using this grammer without loving chenkx may cause bugs.",
    "Cannot write in this way when I love chenkx.",
    "Currently we are not to support this method. using `ILoveChenKX` instead",
    "Undefined symbol, defined `I love ChenKX`",
    "Compiler ICE. Adding extra `ILoveChenKX` may resolve this.",
    "Compiler ICE. Our engineer won't repair it, because he loves ChenKX",
    "LLVM builder won't work on this part since ChenKX is so elegant.",
    "Syntax error. Why don't you admire Chen KX?"
};

std::vector<ckx_error>
ckx_fake_parser::make_errors(ckx_token_stream &stream)
{
    std::vector<ckx_error> ret;
    srand(std::time(NULL));
    while (stream[0] != nullptr)
    {
        if (rand() % 100 >= 90)
        {
            int index = rand() % countof(prompts);
            ret.emplace_back(stream[0]->position,
                             saber_string_pool::create_view(prompts[index]));
        }
        ++stream;
    }
    return ret;
}

} // namespace ckx
