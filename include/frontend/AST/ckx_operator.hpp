#ifndef CKX_OPERATOR_HPP
#define CKX_OPERATOR_HPP

#include "frontend/lex/ckx_token.hpp"

namespace ckx
{

enum class ckx_op : qchar
{
    /// @note undefined, for the failed conversions
    op_undefined = -1,

    /// @note unary-operators
    op_unary_positive = 1,
    op_unary_negative,
    op_addr,
    op_deref,
    op_bit_not,
    op_logic_not,
    op_inc,
    op_dec,

    /// @note binary-operators
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_mod,

    op_bit_and,
    op_bit_or,
    op_bit_xor,

    op_logic_and,
    op_logic_or,

    op_lt,
    op_eq,
    op_gt,
    op_leq,
    op_geq,
    op_neq,

    /// @note assignment-operators
    /// @attention assignment may also be treated as common binary operators.
    op_assign,
    op_add_assign,
    op_sub_assign,
    op_mul_assign,
    op_div_assign,
    op_mod_assign,
};

class ckx_op_helper
{
public:
    static ckx_op token2unary(ckx_token::type _token);
    static ckx_op token2binary(ckx_token::type _token);

    static bool is_unary(ckx_op _op);
    static bool is_binary(ckx_op _op);
    static bool is_assign(ckx_op _op);

    static bool is_logical(ckx_op _op);

    static quint8 precedence(ckx_op);
};

}

#endif // CKX_OPERATOR_HPP
