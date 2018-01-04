#ifndef CKX_SEMA_HPP
#define CKX_SEMA_HPP

#include "../llvm_ir_builder/llvm_ir_builder.hpp"
#include "ckx_env_table.hpp"

namespace ckx
{


class ckx_sema_engine
{
public:

private:
    faker::llvm_ir_builder ir_builder;
    ckx_env env_holder;
};

} // namespace ckx

#endif // CKX_SEMA_HPP
