#ifndef CKX_SEMA_RESULT_FWD_HPP
#define CKX_SEMA_RESULT_FWD_HPP

#include "memory.hpp"

namespace ckx
{

constexpr size_t ckx_expr_result_size = 24;
using ckx_expr_result_storage = saber::aligned_storage_t<24, 8>;

};

#endif // CKX_SEMA_RESULT_FWD_HPP
