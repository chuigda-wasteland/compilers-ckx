#include "ckx_operator.hpp"

namespace ckx
{

ckx_op ckx_op_helper::token2unary(ckx_token::type _token)
{
    switch (_token)
    {
    default: return ckx_op::op_undefined;
    }
}

ckx_op ckx_op_helper::token2binary(ckx_token::type _token)
{
    switch (_token)
    {
    default: return ckx_op::op_undefined;
    }
}

bool ckx_op_helper::is_unary(ckx_op _op)
{
    Q_UNUSED(_op)
    return false;
}

bool ckx_op_helper::is_binary(ckx_op _op)
{
    Q_UNUSED(_op)
    return false;
}

bool ckx_op_helper::is_assign(ckx_op _op)
{
    Q_UNUSED(_op)
    return false;
}

}
