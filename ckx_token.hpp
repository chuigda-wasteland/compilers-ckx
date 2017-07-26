#ifndef CKX_TOKEN_HPP
#define CKX_TOKEN_HPP

#include "defs.hpp"
#include "string.hpp"

namespace ckx
{

struct ckx_token
{
    enum class ckx_token_type : unsigned char
    {
    };

    ckx_token() = delete;
    ckx_token(const ckx_token&) = delete;
    ~ckx_token();

    ckx_token& operator= (const ckx_token&) = delete;

    ckx_token_type token_type;

    union
    {
        qchar ch;

        qint8 i8;
        qint16 i16;
        qint32 i32;
        qint64 i64;

        quint8 u8;
        quint16 u16;
        quint32 u32;
        quint64 u64;

        saber::string *p_str;
    }
    v;
};

}

#endif // CKX_TOKEN_HPP
