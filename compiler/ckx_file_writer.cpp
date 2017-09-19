#include "ckx_file_writer.hpp"
#include <iostream>
#include <inttypes.h>

#include "utility.hpp"

namespace ckx
{

namespace detail
{
class ckx_fp_writer_impl
{
public:
    explicit ckx_fp_writer_impl(std::FILE* _fp);
    ~ckx_fp_writer_impl() = default;

    inline void write_impl(qint64 _value);
    inline void write_impl(quint64 _value);
    inline void write_impl(qreal _value);
    inline void write_impl(const qchar* _str);
    inline void write_whitespace_impl(qsizet _count);

private:
    std::FILE* fp;
};

class ckx_ostream_writer_impl
{
public:
    explicit ckx_ostream_writer_impl(std::ostream &_stream);
    ~ckx_ostream_writer_impl() = default;

    template <typename AnyType> inline void write_impl(AnyType&& _any_v);
    inline void write_whitespace_impl(qsizet _count);

private:
    std::ostream& stream;
};

} // namespace detail


ckx_file_writer::~ckx_file_writer()
{}


ckx_fp_writer::ckx_fp_writer(std::FILE *_fp)
{
    impl = new detail::ckx_fp_writer_impl(_fp);
}

ckx_fp_writer::~ckx_fp_writer()
{
    delete impl;
}

void ckx_fp_writer::write(qint64 _value)
{
    impl->write_impl(qint64(_value));
}

void ckx_fp_writer::write(quint64 _value)
{
    impl->write_impl(quint64(_value));
}

void ckx_fp_writer::write(qreal _value)
{
    impl->write_impl(_value);
}

void ckx_fp_writer::write(const qchar *_str)
{
    impl->write_impl(_str);
}

void ckx_fp_writer::write_whitespace(qsizet _count)
{
    impl->write_whitespace_impl(_count);
}


ckx_ostream_writer::ckx_ostream_writer(std::ostream &_stream)
{
    impl = new detail::ckx_ostream_writer_impl(_stream);
}

ckx_ostream_writer::~ckx_ostream_writer()
{
    delete impl;
}

void ckx_ostream_writer::write(qint64 _value)
{
    impl->write_impl(qint64(_value));
}

void ckx_ostream_writer::write(quint64 _value)
{
    impl->write_impl(quint64(_value));
}

void ckx_ostream_writer::write(qreal _value)
{
    impl->write_impl(_value);
}

void ckx_ostream_writer::write(const qchar *_str)
{
    impl->write_impl(_str);
}

void ckx_ostream_writer::write_whitespace(qsizet _count)
{
    impl->write_whitespace_impl(_count);
}


namespace detail
{

ckx_fp_writer_impl::ckx_fp_writer_impl(std::FILE *_fp) :
    fp(_fp)
{}

inline void ckx_fp_writer_impl::write_impl(qint64 _value)
{
    std::fprintf(fp, "%" PRId64, _value);
}

inline void ckx_fp_writer_impl::write_impl(quint64 _value)
{
    std::fprintf(fp, "%" PRIu64, _value);
}

inline void ckx_fp_writer_impl::write_impl(qreal _value)
{
    std::fprintf(fp, "%lf", _value);
}

inline void ckx_fp_writer_impl::write_impl(const qchar *_str)
{
    std::fprintf(fp, "%s", _str);
}

inline void ckx_fp_writer_impl::write_whitespace_impl(qsizet _count)
{
    for (qsizet i = 0; i < _count; i++) std::fputc(' ', fp);
}


ckx_ostream_writer_impl::ckx_ostream_writer_impl(std::ostream &_stream) :
    stream(_stream)
{}

template <typename AnyType>
inline void ckx_ostream_writer_impl::write_impl(AnyType&& _any_v)
{
    stream << saber::forward<AnyType>(_any_v);
}

inline void ckx_ostream_writer_impl::write_whitespace_impl(qsizet _count)
{
    for (qsizet i = 0; i < _count; i++) stream.put(' ');
}

} // namespace detail

} // namespace ckx
