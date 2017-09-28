#include "ckx_operator.hpp"

namespace ckx
{

ckx_op ckx_op_helper::token2unary(ckx_token::type _token)
{
    switch (_token)
    {
    case ckx_token::type::tk_add:        return ckx_op::op_unary_positive;
    case ckx_token::type::tk_sub:        return ckx_op::op_unary_negative;
    case ckx_token::type::tk_bit_and:    return ckx_op::op_addr;
    case ckx_token::type::tk_mul:        return ckx_op::op_deref;
    case ckx_token::type::tk_bit_not:    return ckx_op::op_bit_not;
    case ckx_token::type::tk_logic_not:  return ckx_op::op_logic_not;
    case ckx_token::type::tk_inc:        return ckx_op::op_inc;
    case ckx_token::type::tk_dec:        return ckx_op::op_dec;

    default: return ckx_op::op_undefined;
    }
}

ckx_op ckx_op_helper::token2binary(ckx_token::type _token)
{
    switch (_token)
    {
    case ckx_token::type::tk_add:  return ckx_op::op_add;
    case ckx_token::type::tk_sub:  return ckx_op::op_sub;
    case ckx_token::type::tk_mul:  return ckx_op::op_mul;
    case ckx_token::type::tk_div:  return ckx_op::op_div;
    case ckx_token::type::tk_mod:  return ckx_op::op_mod;
    case ckx_token::type::tk_bit_and:    return ckx_op::op_bit_and;
    case ckx_token::type::tk_bit_or:     return ckx_op::op_bit_or;
    case ckx_token::type::tk_bit_xor:    return ckx_op::op_bit_xor;
    case ckx_token::type::tk_logic_and:  return ckx_op::op_logic_and;
    case ckx_token::type::tk_logic_or:   return ckx_op::op_logic_or;
    case ckx_token::type::tk_lt:   return ckx_op::op_lt;
    case ckx_token::type::tk_eq:   return ckx_op::op_eq;
    case ckx_token::type::tk_gt:   return ckx_op::op_gt;
    case ckx_token::type::tk_leq:  return ckx_op::op_leq;
    case ckx_token::type::tk_geq:  return ckx_op::op_geq;
    case ckx_token::type::tk_neq:  return ckx_op::op_neq;

    /// @attention now we treat all assignments as binary operators.
    case ckx_token::type::tk_assign:      return ckx_op::op_assign;
    case ckx_token::type::tk_add_assign:  return ckx_op::op_add_assign;
    case ckx_token::type::tk_sub_assign:  return ckx_op::op_sub_assign;
    case ckx_token::type::tk_mul_assign:  return ckx_op::op_mul_assign;
    case ckx_token::type::tk_div_assign:  return ckx_op::op_div_assign;
    case ckx_token::type::tk_mod_assign:  return ckx_op::op_mod_assign;

    default: return ckx_op::op_undefined;
    }
}

bool ckx_op_helper::is_unary(ckx_op _op)
{
    return (_op >= ckx_op::op_unary_positive
            && _op <= ckx_op::op_dec);
}

bool ckx_op_helper::is_binary(ckx_op _op)
{
    return (_op >= ckx_op::op_add
            && _op <= ckx_op::op_mod_assign);
}

bool ckx_op_helper::is_assign(ckx_op _op)
{
    return (_op >= ckx_op::op_assign
            && _op <= ckx_op::op_mod_assign);
}

quint8 ckx_op_helper::precedence(ckx_op _op)
{
    switch (_op)
    {
    case ckx_op::op_unary_positive:
    case ckx_op::op_unary_negative:
    case ckx_op::op_addr:
    case ckx_op::op_deref:
    case ckx_op::op_bit_not:
    case ckx_op::op_logic_not:
    case ckx_op::op_inc:
    case ckx_op::op_dec:
        return 64;

    case ckx_op::op_mul:
    case ckx_op::op_div:
    case ckx_op::op_mod:
        return 60;

    case ckx_op::op_add:
    case ckx_op::op_sub:
        return 56;

    case ckx_op::op_lt:
    case ckx_op::op_leq:
    case ckx_op::op_gt:
    case ckx_op::op_geq:
        return 48;

    case ckx_op::op_eq:
    case ckx_op::op_neq:
        return 44;

    case ckx_op::op_bit_and:
        return 40;

    case ckx_op::op_bit_or:
        return 36;

    case ckx_op::op_logic_and:
        return 32;

    case ckx_op::op_logic_or:
        return 28;

    case ckx_op::op_assign:
    case ckx_op::op_add_assign:
    case ckx_op::op_sub_assign:
    case ckx_op::op_mul_assign:
    case ckx_op::op_div_assign:
    case ckx_op::op_mod_assign:
        return 24;

    default:
        assert(false); // What the fuck!
    }
}

} // namespace ckx
