#ifndef CKX_PARSER_HPP
#define CKX_PARSER_HPP

#include "ckx_token_stream.hpp"
#include "ckx_ast_node.hpp"

namespace ckx
{

template <typename TokenStream>
class ckx_parser
{
public:
    ckx_parser();
    ~ckx_parser();

    ckx_ast_node* parse(TokenStream& _token_stream);
};

extern template class ckx_parser<ckx_default_token_stream>;

} // namespace ckx

#endif // CKX_PARSER_HPP
