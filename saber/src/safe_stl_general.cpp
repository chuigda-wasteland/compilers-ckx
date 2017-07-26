#include "safe_stl_general.hpp"

namespace saber
{

static FILE *fp = stderr;

FILE *& fp_export(void)
{
    return fp;
}

void set_export(FILE *_fp)
{
    fp_export() = _fp;
}

}
