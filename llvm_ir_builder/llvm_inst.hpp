#ifndef LLVM_INST_HPP
#define LLVM_INST_HPP

#include "defs.hpp"

namespace faker
{

interface llvm_instruction
{
public:
    ~llvm_instruction() = 0;
};

} // namespace faker

#endif // LLVM_INST_HPP
