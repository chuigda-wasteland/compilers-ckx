#ifndef CKX_ERROR_INTEPRETER_H
#define CKX_ERROR_INTEPRETER_H

#include "file_reader.hpp"
#include "vector.hpp"
#include "string_pool.hpp"

#include "ckx_error.hpp"

namespace ckx
{

class ckx_error_intepreter
{
public:
    ckx_error_intepreter(we::we_file_reader& _reader);
    void interpret(const ckx_error& _error);

private:
    saber::vector<saber_string> lines;
};

} // namespace ckx

#endif // CKX_ERROR_INTEPRETER_H
