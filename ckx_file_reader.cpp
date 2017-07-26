#include "ckx_file_reader.hpp"
#include <iostream>

using namespace ckx;

ckx_file_reader::~ckx_file_reader() {}

namespace ckx
{

namespace detail
{

class ckx_fp_reader_impl
{
public:
    ckx_fp_reader_impl(std::FILE* _fp);
    ~ckx_fp_reader_impl() = default;

    char get_next_char_impl();

private:
    std::FILE *fp;
};


class ckx_istream_reader_impl
{
public:
    ckx_istream_reader_impl(std::istream& _stream);
    ~ckx_istream_reader_impl() = default;

    char get_next_char_impl();

private:
    std::istream& stream;
};

} // namespace detail

ckx_fp_reader::ckx_fp_reader(FILE *_fp)
{
    this->impl = new detail::ckx_fp_reader_impl(_fp);
}

ckx_fp_reader::~ckx_fp_reader()
{
    delete this->impl;
}

char ckx_fp_reader::get_next_char()
{
    return this->impl->get_next_char_impl();
}


ckx_istream_reader::ckx_istream_reader(std::istream &_stream)
{
    this->impl = new detail::ckx_istream_reader_impl(_stream);
}

ckx_istream_reader::~ckx_istream_reader()
{
    delete this->impl;
}

char ckx_istream_reader::get_next_char()
{
    return this->impl->get_next_char_impl();
}

namespace detail
{

ckx_fp_reader_impl::ckx_fp_reader_impl(FILE *_fp) : fp(_fp) {}
char ckx_fp_reader_impl::get_next_char_impl()
{
    char ret;
    std::fscanf(this->fp, "%c", &ret);
    return ret;
}


ckx_istream_reader_impl::ckx_istream_reader_impl(std::istream &_stream) :
    stream(_stream)
{}

char ckx_istream_reader_impl::get_next_char_impl()
{
    char ch;
    return stream >> ch ? ch : std::char_traits<char>::eof();
}


} // namespace detail

} // namespace ckx
