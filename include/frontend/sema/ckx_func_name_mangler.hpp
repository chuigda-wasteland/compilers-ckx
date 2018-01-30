#ifndef CKX_FUNC_NAME_MANGLER_HPP
#define CKX_FUNC_NAME_MANGLER_HPP

#include "frontend/sema/ckx_type.hpp"
#include "saber/string_pool.hpp"

namespace ckx
{

class ckx_func_name_mangler
{
public:
    static saber_string_view std_mangle(saber_string_view _func_name,
                                        ckx_type* _func_type);
    static saber_string_view simple_mangle(saber_string_view _func_name);
    static saber_string_view prime(saber_string_view _func_name);
};

};

#endif // CKX_FUNC_NAME_MANGLER_HPP
