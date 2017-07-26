#ifndef CKX_TOKEN_STREAM_HPP
#define CKX_TOKEN_STREAM_HPP

#include "ckx_token.hpp"

#include "memory.hpp"
#include "string.hpp"

namespace ckx
{

using saber::saber_ptr;


class ckx_token_stream
{
public:
    ckx_token_stream() = default;
    virtual ~ckx_token_stream() = 0;

    virtual saber_ptr<ckx_token> operator[] (int _offset) = 0;
    virtual void operator++ () = 0;
};


namespace detail
{
class ckx_default_token_stream_impl;
} // namespace detail


class ckx_default_token_stream : public ckx_token_stream
{
public:
    ckx_default_token_stream(const saber::string& _filename);
    ~ckx_default_token_stream();

    saber_ptr<ckx_token> operator[] (int _offset) override final;
    void operator++ () override final;

private:
    detail::ckx_default_token_stream_impl *impl;
};

} // namespace ckx

#endif // CKX_TOKEN_STREAM_HPP
