#ifndef CKX_FILE_READER_HPP
#define CKX_FILE_READER_HPP

#include <iosfwd>
#include <string>

#include <cstdio>

namespace ckx
{
namespace detail
{
    class ckx_fp_reader_impl;
    class ckx_istream_reader_impl;
} // namespace detail


class ckx_file_reader
{
public:
    ckx_file_reader() = default;
    virtual ~ckx_file_reader() = 0;

    virtual char get_next_char(void) = 0;
};


class ckx_fp_reader : public ckx_file_reader
{
public:
    ckx_fp_reader(std::FILE* _fp);
    ~ckx_fp_reader() override;
    char get_next_char() override final;

private:
    detail::ckx_fp_reader_impl *impl;
};


class ckx_istream_reader : public ckx_file_reader
{
public:
    ckx_istream_reader(std::istream& _stream);
    ~ckx_istream_reader() override;
    char get_next_char() override final;

private:
    detail::ckx_istream_reader_impl* impl;
};

} // namespace ckx

#endif // CKX_FILE_READER_HPP
