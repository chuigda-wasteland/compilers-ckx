#include "ckx_error_intepreter.h"
#include <stdio.h>

namespace ckx
{

ckx_error_intepreter::ckx_error_intepreter(we::we_file_reader& _reader)
{
    char ch;
    saber_string str = "";
    while ((ch = _reader.get_next_char()) !=
           std::char_traits<qchar>::eof())
    {
        if (ch != '\n')
        {
            str.append(1, ch);
        }
        else
        {
            lines.push_back(str);
            str = "";
        }
    }
}

void
ckx_error_intepreter::interpret(const ckx_error &_error)
{
    if (_error.pos.first > lines.size()-1) return;
    if (_error.pos.second > lines[_error.pos.first].size()) return;
    printf("Error at line %d, column %d : %s\n",
           _error.pos.first,
           _error.pos.second,
           _error.desc.get().c_str());
    printf("%s\n", lines[_error.pos.first].c_str());

    for (int i = 0; i < _error.pos.second; i++)
        putchar(' ');

    printf("^\n");
}

} // namespace ckx
