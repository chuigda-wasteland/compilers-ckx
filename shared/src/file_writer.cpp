#include "file_writer.hpp"
#include <iostream>
#include <inttypes.h>

#include "utility.hpp"

namespace we
{

namespace detail
{
class we_fp_writer_impl
{
public:
    explicit we_fp_writer_impl(std::FILE* _fp);
    ~we_fp_writer_impl() = default;

    inline void write_impl(qint64 _value);
    inline void write_impl(quint64 _value);
    inline void write_impl(qreal _value);
    inline void write_impl(const qchar* _str);
    inline void write_impl(const saber_string& _str);
    inline void write_impl(saber_string_view _str_view);
    inline void write_whitespace_impl(qsizet _count);

private:
    std::FILE* fp;
};

class we_ostream_writer_impl
{
public:
    explicit we_ostream_writer_impl(std::ostream &_stream);
    ~we_ostream_writer_impl() = default;

    template <typename AnyType> inline void write_impl(AnyType&& _any_v);
    inline void write_impl(saber_string_view _str);
    inline void write_whitespace_impl(qsizet _count);

private:
    std::ostream& stream;
};

} // namespace detail


we_file_writer::~we_file_writer()
{}


we_fp_writer::we_fp_writer(std::FILE *_fp)
{
    impl = new detail::we_fp_writer_impl(_fp);
}

we_fp_writer::~we_fp_writer()
{
    delete impl;
}

void we_fp_writer::write(qint64 _value)
{
    impl->write_impl(qint64(_value));
}

void we_fp_writer::write(quint64 _value)
{
    impl->write_impl(quint64(_value));
}

void we_fp_writer::write(qreal _value)
{
    impl->write_impl(_value);
}

void we_fp_writer::write(const qchar *_str)
{
    impl->write_impl(_str);
}

void we_fp_writer::write(const saber_string &_str)
{
    impl->write_impl(_str);
}

void we_fp_writer::write(saber_string_view _str_view)
{
    impl->write_impl(_str_view);
}

void we_fp_writer::write_whitespace(qsizet _count)
{
    impl->write_whitespace_impl(_count);
}


we_ostream_writer::we_ostream_writer(std::ostream &_stream)
{
    impl = new detail::we_ostream_writer_impl(_stream);
}

we_ostream_writer::~we_ostream_writer()
{
    delete impl;
}

void we_ostream_writer::write(qint64 _value)
{
    impl->write_impl(qint64(_value));
}

void we_ostream_writer::write(quint64 _value)
{
    impl->write_impl(quint64(_value));
}

void we_ostream_writer::write(qreal _value)
{
    impl->write_impl(_value);
}

void we_ostream_writer::write(const qchar *_str)
{
    impl->write_impl(_str);
}

void we_ostream_writer::write(const saber_string &_str)
{
    impl->write_impl(_str);
}

void we_ostream_writer::write(saber_string_view _str_view)
{
    impl->write_impl(_str_view);
}

void we_ostream_writer::write_whitespace(qsizet _count)
{
    impl->write_whitespace_impl(_count);
}


namespace detail
{

we_fp_writer_impl::we_fp_writer_impl(std::FILE *_fp) :
    fp(_fp)
{}

inline void we_fp_writer_impl::write_impl(qint64 _value)
{
    std::fprintf(fp, "%" PRId64, _value);
}

inline void we_fp_writer_impl::write_impl(quint64 _value)
{
    std::fprintf(fp, "%" PRIu64, _value);
}

inline void we_fp_writer_impl::write_impl(qreal _value)
{
    std::fprintf(fp, "%lf", _value);
}

inline void we_fp_writer_impl::write_impl(const qchar *_str)
{
    std::fprintf(fp, "%s", _str);
}

inline void we_fp_writer_impl::write_impl(const saber_string &_str)
{
    std::fprintf(fp, "%s", _str.c_str());
}

inline void we_fp_writer_impl::write_impl(saber_string_view _str_view)
{
    std::fprintf(fp, "%s", _str_view.get().c_str());
}

inline void we_fp_writer_impl::write_whitespace_impl(qsizet _count)
{
    for (qsizet i = 0; i < _count; i++) std::fputc(' ', fp);
}


we_ostream_writer_impl::we_ostream_writer_impl(std::ostream &_stream) :
    stream(_stream)
{}

template <typename AnyType>
inline void we_ostream_writer_impl::write_impl(AnyType&& _any_v)
{
    stream << saber::forward<AnyType>(_any_v);
}

inline void we_ostream_writer_impl::write_impl(saber_string_view _str_view)
{
    stream << _str_view.get();
}

inline void we_ostream_writer_impl::write_whitespace_impl(qsizet _count)
{
    for (qsizet i = 0; i < _count; i++) stream.put(' ');
}

} // namespace detail

} // namespace ckx
