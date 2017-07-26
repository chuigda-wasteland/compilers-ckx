#include "ckx_token_stream.hpp"

#include "ckx_file_reader.hpp"
#include "vector.hpp"

namespace ckx
{
namespace detail
{

class ckx_default_token_stream_impl
{
public:
    ckx_default_token_stream_impl(const saber::string& _filename);
    ~ckx_default_token_stream_impl();

    saber_ptr<ckx_token> operator_index_impl(int _offset);
    void operator_incr_impl(void);
};

}

ckx_default_token_stream::ckx_default_token_stream(
        const saber::string &_filename)
{
    impl = new detail::ckx_default_token_stream_impl(_filename);
}

ckx_default_token_stream::~ckx_default_token_stream()
{
    delete impl;
}

saber_ptr<ckx_token> ckx_default_token_stream::operator [](int _offset)
{
    return impl->operator_index_impl(_offset);
}

void ckx_default_token_stream::operator ++ ()
{
    impl->operator_incr_impl();
}

} // namespace cks
