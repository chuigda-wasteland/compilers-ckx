#include "ckx_parser_impl.hpp"
#include "memory.hpp"

using namespace ckx;

template <typename CkxTokenStream>
class ckx_parser_impl_test final :
        public detail::ckx_parser_impl<CkxTokenStream>
{
public:
    void initialize_test(saber_ptr<CkxTokenStream> _stream);
    void cleanup_test();
};

int main()
{
    ckx_parser_impl_test<ckx_default_token_stream> test;
    return 0;
}
