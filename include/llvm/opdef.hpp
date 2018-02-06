
#ifndef BINOP
#define BINOP(OP_NAME)
#endif


#ifndef CASTOP
#define CASTOP(OP_NAME)
#endif


#ifndef CMPOP
#define CMPOP(OP_NAME)
#endif


BINOP(add)
BINOP(sub)
BINOP(mul)
BINOP(sdiv)
BINOP(udiv)
BINOP(srem)
BINOP(urem)
BINOP(fadd)
BINOP(fsub)
BINOP(fmul)
BINOP(fdiv)
BINOP(shl)
BINOP(lshr)
BINOP(ashr)
BINOP(and)
BINOP(or)
BINOP(xor)


CASTOP(trunc)
CASTOP(zext)
CASTOP(sext)
CASTOP(fpext)
CASTOP(fptoui)
CASTOP(uitofp)
CASTOP(inttoptr)
CASTOP(ptrtoint)
CASTOP(bitcast)


CMPOP(icmp_ne)
CMPOP(icmp_eq)
CMPOP(icmp_sgt)
CMPOP(icmp_sge)
CMPOP(icmp_slt)
CMPOP(icmp_sle)
CMPOP(icmp_ugt)
CMPOP(icmp_uge)
CMPOP(icmp_ult)
CMPOP(icmp_ule)
CMPOP(fcmp_oeq)
CMPOP(fcmp_one)
CMPOP(fcmp_ogt)
CMPOP(fcmp_oge)
CMPOP(fcmp_olt)
CMPOP(fcmp_ole)
