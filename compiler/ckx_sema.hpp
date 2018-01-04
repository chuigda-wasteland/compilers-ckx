#ifndef CKX_SEMA_HPP
#define CKX_SEMA_HPP

namespace ckx
{

namespace detail
{
class ckx_sema_engine_impl;
} // namespace detail

class ckx_sema_engine
{
public:
private:
    detail::ckx_sema_engine_impl *pimpl;
};

} // namespace ckx

#endif // CKX_SEMA_HPP
