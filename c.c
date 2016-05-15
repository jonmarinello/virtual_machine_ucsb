# include "vm.h"

ccc() {
	no_op = TRUE;
	psw.nbit = psw.zbit = psw.vbit = psw.cbit = 0;
	answer = 0;
}


clc() {
	no_op = TRUE;
	psw.cbit = 0;
	answer = 0;
}


cln() {
	no_op = TRUE;
	psw.nbit = 0;
	answer = 0;
}


clr() {
	int	*dd;

	get_destination(&dd);
	answer = *dd = 0;
	psw.nbit = psw.vbit = psw.cbit = 0;
	psw.zbit = 1;
}


clrb() {
	int	*dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	answer = *byte_1;
	psw.nbit = 0;
	psw.zbit = 1;
	psw.vbit = psw.cbit = 0;
}


clv() {
	no_op = TRUE;
	psw.vbit = 0;
	answer = 0;
}


clz() {
	no_op = TRUE;
	psw.zbit = 0;
	answer = 0;
}


cmp() {				/* word (src) - (dst) */
	int	*op1, *op2;
	long	int	result;

	two_op = TRUE;
	get_source_dest(&op1, &op2);
	result = (long)*op1 - (long)*op2;
	answer = (int)result;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	if (result > 32767L || result < -32768L)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	if (result & 0277777L)
		psw.cbit = 0;
}


cmpb() {			/* byte (src) - (dst) */
	int	*op1, *op2;
	char	*byte_1, *byte_2;

	byte_op = two_op = TRUE;
	get_source_dest(&op1, &op2);
	byte_1 = (char *)op1;
	byte_2 = (char *)op2;
	if (usehi_1)
		++byte_1;
	if (usehi_2)
		++byte_2;
	answer = *byte_1 - *byte_2;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
	if (answer & 000400)
		psw.cbit = 0;
}


com() {
	int	*dd;

	get_destination(&dd);
	answer = *dd = ~*dd;
	psw.nbit = ((!answer & (1 << 16)) ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
	psw.cbit = 1;
}


comb() {
	int	*dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	answer = *byte_1 = ~*byte_1;
	psw.nbit = ((!*byte_1 & (1 << 8)) ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
	psw.cbit = 1;
}
