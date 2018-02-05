#ifndef CKX_LLVM_TYPE_BUILDER_HPP
#define CKX_LLVM_TYPE_BUILDER_HPP

#include "frontend/sema/ckx_type.hpp"
#include "llvm/llvm_type.hpp"

namespace ckx
{

class ckx_llvm_type_builder final
{
public:
    static faker::llvm_type build(ckx_type *_type);
    static saber_string_view full_name_for_struct(saber_string_view _origin);
    static saber_string_view full_name_for_variant(saber_string_view _origin);
};

} // namespace ckx

#endif // CKX_LLVM_TYPE_BUILDER_HPP
