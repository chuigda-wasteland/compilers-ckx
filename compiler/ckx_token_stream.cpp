/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#include "ckx_token_stream.hpp"

#include "file_reader.hpp"
#include "vector.hpp"
#include "c8assert.hpp"

namespace ckx
{

namespace detail
{

class ckx_identifier_table
{
public:
    static qpair<ckx_token::type, bool> lookup(saber_string_view _name);

private:
    static saber::unordered_map<saber_string, ckx_token::type> map;
};

saber::unordered_map<saber_string, ckx_token::type> ckx_identifier_table::map
{
#define GGLEX(X, Y)\
    qpair<saber_string, ckx_token::type>(X, ckx_token::type::tk_##Y),
#include "gg.h"
#undef GGLEX
    qpair<saber_string, ckx_token::type>("{-# ice1000 #-}",
                                         ckx_token::type::tk_eoi)
};



class ckx_token_stream_impl final
{
public:
    explicit ckx_token_stream_impl(we::we_file_reader &_file_reader);
    ~ckx_token_stream_impl();

    ckx_token operator_index_impl(int _offset);
    saber::vector<ckx_error>& get_error_impl(void);
    void operator_incr_impl(void);

private:
    void do_split_tokens();

    void solve_numbers();
    void solve_char_literal();
    void solve_bitwise_or_logic_op();
    void solve_add_n_sub();
    void solve_op_or_opassign();
    void solve_colon_or_scope();
    void solve_array_or_lbracket();
    void solve_ordinary_op();
    void scan_full_id_string();
    bool solve_keyword();
    void solve_identifier();

    qint64 scan_integer();
    qreal scan_floating_part();
    qchar make_conversion();

    inline bool is_part_of_id(qchar _ch);
    qchar ch();
    inline saber_string_view str();
    inline qcoord& char_coord();
    inline void next_char();

    inline void lex_error(saber_string &&);
    inline void lex_warn(saber_string &&);

private:
    we::we_file_reader& reader;
    qsizet current_pos;
    qcoord char_coord_temp;
    saber_string_view string_temp;

    saber::vector<ckx_token> token_buffer;
    saber::vector<ckx_error> errors;

    qchar char_temp;
};

}

ckx_token_stream::ckx_token_stream(we::we_file_reader &_file_reader)
{
    impl = new detail::ckx_token_stream_impl(_file_reader);
}

ckx_token_stream::~ckx_token_stream()
{
    delete impl;
}

ckx_token
ckx_token_stream::operator[] (int _offset)
{
    return impl->operator_index_impl(_offset);
}

saber::vector<ckx_error>& ckx_token_stream::get_error()
{
    return impl->get_error_impl();
}

void ckx_token_stream::operator++ ()
{
    impl->operator_incr_impl();
}



namespace detail
{

qpair<ckx_token::type, bool>
ckx_identifier_table::lookup(saber_string_view _name)
{

    if (map.find(_name.get()) != map.end())
    {
        return qpair<ckx_token::type, bool>(map[_name.get()], true);
    }
    else
    {
        return qpair<ckx_token::type, bool>(ckx_token::type::tk_eoi, false);
    }
}



ckx_token_stream_impl::ckx_token_stream_impl(
        we::we_file_reader &_file_reader) :
    reader(_file_reader),
    current_pos(0),
    char_coord_temp(1, 1),
    string_temp(saber_string_pool::create_view(""))
{
    next_char();
    do_split_tokens();
}

ckx_token_stream_impl::~ckx_token_stream_impl()
{}

ckx_token
ckx_token_stream_impl::operator_index_impl(int _offset)
{
    if ( _offset + current_pos < token_buffer.size() )
    {
        return token_buffer[current_pos + _offset];
    }
    else
    {
        return ckx_token(char_coord(), ckx_token::type::tk_eoi);
    }
}

saber::vector<ckx_error> &ckx_token_stream_impl::get_error_impl()
{
    return errors;
}

void ckx_token_stream_impl::operator_incr_impl()
{
    current_pos++;
}



void ckx_token_stream_impl::do_split_tokens()
{
    while ( ch() != static_cast<qchar>(saber::char_traits<qchar>::eof()) )
    {
        switch (ch())
        {
        case '\n':
            next_char(); break;

        case '\f': case '\v': case '\t': case '\r': case ' ':
            next_char(); break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            solve_numbers(); break;

        case '\'':
            solve_char_literal(); break;

        case '&': case '|': case '^':
            solve_bitwise_or_logic_op(); break;

        case '+': case '-':
            /// @details now + and - must be solved separately.
            solve_add_n_sub(); break;

        case '*': case '/': case '%': case '!': case '<':
        case '>': case '=':
            /// @details
            /// the processing of >=, <=, != and == is similar to op=,
            /// so we combine them in one function solve_op_or_opassign().
            solve_op_or_opassign(); break;

        case ':':
            solve_colon_or_scope(); break;

        case '~': case ';': case ',': case '.': case '{':
        case '}': case ']': case '(': case ')': case '?':
            solve_ordinary_op(); break;

        case '[':
            solve_array_or_lbracket(); break;

        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'i': case 'o': case 'r': case 's':
        case 'v': case 'w':
            scan_full_id_string();
            if (!solve_keyword()) goto ordinary_identifier_tag;
            break;

        case 'g': case 'h': case 'j': case 'k': case 'l':
        case 'm': case 'n': case 'p': case 'q': case 't':
        case 'u': case 'x': case 'y': case 'z': case 'A':
        case 'B': case 'C': case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I': case 'J': case 'K':
        case 'L': case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
            scan_full_id_string();
        ordinary_identifier_tag:
            solve_identifier(); break;

        default:
            lex_error("Unrecognized character");
            next_char();
        }
    }

    token_buffer.emplace_back(char_coord(), ckx_token::type::tk_eoi);
}

/**
    @brief function ckx_token_stream_impl::solve_$xxxxx();

    These functions would start at the first character of a lexeme
    with the assertion that there must be a targeted lexeme. And they
    stops at the first character that does not belong to the
    targeted lexeme. For example

    @example
        Char stream

        vr64     pi = 3.1415926;
        ^~~~
        scan_full_id_string & solve_keyword

        vr64     pi = 3.1415926;
            ^~~~~
            next_char

        vr64     pi = 3.1415926;
                 ^~
                 scan_full_id_string & solve_identifier

        vr64     pi = 3.1415926;
                   ^
                   next_char

        vr64     pi = 3.1415926;
                    ^
                    solve_op_or_assign

        vr64     pi = 3.1415926;
                     ^
                     next_char

        vr64     pi = 3.1415926;
                      ^~~~~~~~~
                      solve_number

        vr64     pi = 3.1415926;
                               ^
                               solve_ordinary_op

        vr64     pi = 3.1415926;
                                ^next_line
*/


/**
    @brief ckx_token_stream_impl::solve_numbers
    This function solves numbers nomatter what kind it is. And the add the
    newly built number token into the token stream.

    @example
        520.1314
        ^~~
        scan_integer

        520.1314
           ^
           next_char

        52.1314
           ^~~~
           scan_floating_part

 */

void ckx_token_stream_impl::solve_numbers()
{
    qint64 i = scan_integer();
    qreal r = i;
    bool is_real = false;

    if (ch() == '.')
    {
        next_char();
        if (ch() < '0' || ch() > '9')
        {
            lex_error("expected digits after floating point.");
            return;
        }

        is_real = true;
        r += scan_floating_part();
    }

    if (ch() == 'E' || ch() == 'e')
    {
        next_char();
        bool is_negative = false;
        if (ch() == '-')
        {
            is_negative = true;
            next_char();
        }

        if ((ch() < '0' || ch() > '9'))
        {
            lex_error("expected digits after exponent.");
            return;
        }

        is_real = true;
        qint64 exponent = scan_integer();
        if (!is_negative) for (qint64 i = 0; i < exponent; i++) r *= 10;
        else for (qint64 i = 0; i < exponent; i++) r /= 10;
    }

    if (is_real)
    {
        token_buffer.emplace_back(char_coord(), r);
    }
    else
    {
        token_buffer.emplace_back(char_coord(), i);
    }
}

void ckx_token_stream_impl::solve_char_literal()
{
    next_char();
    qchar result = (ch()!='\\') ? ch() : make_conversion();

    if (ch() != '\'')
    {
        lex_error("Missing \' character after char literal.");
        return;
    }

    token_buffer.emplace_back(char_coord(), result);
    next_char();
}

void ckx_token_stream_impl::solve_bitwise_or_logic_op()
{
    ckx_token::type new_token_type;
    qchar pre = ch();
    next_char();

    bool is_logic = (pre == ch());

    switch (pre)
    {
    case '&':
        new_token_type = is_logic ? ckx_token::type::tk_logic_and :
                                    ckx_token::type::tk_bit_and;
        break;

    case '|':
        new_token_type = is_logic ? ckx_token::type::tk_logic_or :
                                    ckx_token::type::tk_bit_or;
        break;

    case '^':
        new_token_type = is_logic ? ckx_token::type::tk_logic_xor :
                                    ckx_token::type::tk_bit_xor;
        break;

    default:
        C8ASSERT(false); // what the fuck!
    }

    token_buffer.emplace_back(char_coord(), new_token_type);
    if (is_logic) next_char();
}

void ckx_token_stream_impl::solve_add_n_sub()
{
    ckx_token::type new_token_type;
    qchar op = ch();
    next_char();

    if (ch() == op)
    {
        switch (op)
        {
        case '+': new_token_type = ckx_token::type::tk_inc; break;
        case '-': new_token_type = ckx_token::type::tk_dec; break;
        default: C8ASSERT(false); // What the fuck!
        }
        next_char();
    }
    else if (ch() == '=')
    {
        switch (op)
        {
        case '+': new_token_type = ckx_token::type::tk_add_assign; break;
        case '-': new_token_type = ckx_token::type::tk_sub_assign; break;
        default: C8ASSERT(false); // What the fuck!
        }
        next_char();
    }
    else
    {
        switch (op)
        {
        case '+': new_token_type = ckx_token::type::tk_add; break;
        case '-': new_token_type = ckx_token::type::tk_sub; break;
        default: C8ASSERT(false); // What the fuck!
        }
    }

    token_buffer.emplace_back(char_coord(), new_token_type);
}

void ckx_token_stream_impl::solve_op_or_opassign()
{
    ckx_token::type new_token_type;

    qchar op = ch();
    next_char();
    bool is_opassign = (ch() == '=');

    switch (op)
    {
    case '*':
        new_token_type = is_opassign ? ckx_token::type::tk_mul_assign :
                                       ckx_token::type::tk_mul;
        break;

    case '/':
        new_token_type = is_opassign ? ckx_token::type::tk_div_assign :
                                       ckx_token::type::tk_div;
        break;

    case '%':
        new_token_type = is_opassign ? ckx_token::type::tk_mod_assign :
                                       ckx_token::type::tk_mod;
        break;

    case '!':
        new_token_type = is_opassign ? ckx_token::type::tk_neq :
                                       ckx_token::type::tk_logic_not;
        break;

    case '<':
        new_token_type = is_opassign ? ckx_token::type::tk_leq :
                                       ckx_token::type::tk_lt;
        break;

    case '>':
        new_token_type = is_opassign ? ckx_token::type::tk_geq :
                                       ckx_token::type::tk_gt;
        break;

    case '=':
        new_token_type = is_opassign ? ckx_token::type::tk_eq :
                                       ckx_token::type::tk_assign;
        break;
    }

    token_buffer.emplace_back(char_coord(), new_token_type);
    if (is_opassign) next_char();
}

void ckx_token_stream_impl::solve_colon_or_scope()
{
    next_char();
    if (ch() == ':')
    {
        next_char();
        token_buffer.emplace_back(char_coord(), ckx_token::type::tk_scope);
    }
    else
    {
        token_buffer.emplace_back(char_coord(), ckx_token::type::tk_colon);
    }
}

void ckx_token_stream_impl::solve_array_or_lbracket()
{
    next_char();
    if (ch() == ']')
    {
        next_char();
        token_buffer.emplace_back(char_coord(), ckx_token::type::tk_arr);
    }
    else
    {
        token_buffer.emplace_back(char_coord(), ckx_token::type::tk_lbracket);
    }
}

void ckx_token_stream_impl::solve_ordinary_op()
{
    ckx_token::type new_token_type;

    switch (ch())
    {
    case '~': new_token_type = ckx_token::type::tk_bit_not;    break;
    case ';': new_token_type = ckx_token::type::tk_semicolon;  break;
    case ',': new_token_type = ckx_token::type::tk_comma;      break;
    case '.': new_token_type = ckx_token::type::tk_dot;        break;
    case '{': new_token_type = ckx_token::type::tk_lbrace;     break;
    case '}': new_token_type = ckx_token::type::tk_rbrace;     break;
    case '[': new_token_type = ckx_token::type::tk_lbracket;   break;
    case ']': new_token_type = ckx_token::type::tk_rbracket;   break;
    case '(': new_token_type = ckx_token::type::tk_lparen;     break;
    case ')': new_token_type = ckx_token::type::tk_rparen;     break;
    case '?': new_token_type = ckx_token::type::tk_ques;       break;
        break;
    default:
        // What the fuck!
        C8ASSERT(false);
    }

    token_buffer.emplace_back(char_coord(), new_token_type);
    next_char();
}

/**
    @brief ckx_token_stream_impl::scan_full_id_string

    @details This function will store the scanned string into str().
    And other functions will have to retrieve the lexical value from str().
    Not elegant at all.
 */
void ckx_token_stream_impl::scan_full_id_string()
{
    saber_string strtemp;
    strtemp.clear();
    strtemp.push_back(ch());
    next_char();

    while (is_part_of_id(ch()))
    {
        strtemp.push_back(ch());
        next_char();
    }

    string_temp = saber_string_pool::create_view(saber::move(strtemp));
}

bool ckx_token_stream_impl::solve_keyword()
{
    ckx_token::type type;
    bool is_identifier;
    std::tie(type, is_identifier) = ckx_identifier_table::lookup(str());

    if (is_identifier)
    {
        token_buffer.emplace_back(char_coord(), type);
        return true;
    }
    else
    {
        return false;
    }
}

void ckx_token_stream_impl::solve_identifier()
{
    token_buffer.emplace_back(char_coord(), str());
}



qint64 ckx_token_stream_impl::scan_integer()
{
    qint64 ret = 0;
    bool is_negative = false;
    if (ch() == '-')
    {
        is_negative = true;
        next_char();
    }

    while (ch() >= '0' && ch() <= '9')
    {
        ret *= 10;
        ret += ch() - '0';
        next_char();
    }
    if (is_negative) ret *= -1;
    return ret;
}

qreal ckx_token_stream_impl::scan_floating_part()
{
    qreal ret = 0;
    quint8 digit_count = 0;

    while (ch() >= '0' && ch() <= '9')
    {
        ret *= 10;
        ret += (qreal(ch() - '0'))/10;
        digit_count++;
        next_char();
    }
    ret *= 10;

    for (quint8 i = 0; i < digit_count; i++) ret /= 10;

    return ret;
}

qchar ckx_token_stream_impl::make_conversion()
{
    next_char();
    switch (ch())
    {
    case '0': return '\0';
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 'v': return '\v';

    default:
        // what the fuck!
        lex_error("Illegal conversion sequence");
        return static_cast<qchar>(saber::char_traits<qchar>::eof());
    }
}



inline bool ckx_token_stream_impl::is_part_of_id(qchar _ch)
{
    if (_ch == '_') return true;
    else if (_ch >= 'A' && _ch <= 'Z') return true;
    else if (_ch >= 'a' && _ch <= 'z') return true;
    else if (_ch >= '0' && _ch <= '9') return true;
    else return false;
}

inline qchar ckx_token_stream_impl::ch()
{
    return char_temp;
}

inline saber_string_view ckx_token_stream_impl::str()
{
    return string_temp;
}

inline qcoord& ckx_token_stream_impl::char_coord()
{
    return char_coord_temp;
}

inline void ckx_token_stream_impl::next_char()
{
    char_temp = reader.get_next_char();
    char_coord_temp.second++;
    if (char_temp == '\n')
    {
        char_coord_temp.first++;
        char_coord_temp.second = 1;
    }
}



inline void ckx_token_stream_impl::lex_error(saber_string&& _desc)
{
    errors.emplace_back(
        char_coord(), saber_string_pool::create_view(saber::move(_desc)));
}

inline void ckx_token_stream_impl::lex_warn(saber_string&& _desc)
{
    errors.emplace_back(
        char_coord(), saber_string_pool::create_view(saber::move(_desc)));
}

} // namespace detail

} // namespace ckx
