#ifndef CKX_FAKE_PARSER_H
#define CKX_FAKE_PARSER_H

#include "ckx_token_stream.hpp"
#include "ckx_error.hpp"
#include "vector.hpp"

namespace ckx
{

class ckx_fake_parser
{
public:
    saber::vector<ckx_error> make_errors(ckx_token_stream& stream);
};

} // namespace ckx
#endif // CKX_FAKE_PARSER_H
