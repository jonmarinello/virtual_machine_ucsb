# include "vm.h"

mov() {
	int     *op1;
	int     *op2;

	two_op = TRUE;
	get_source_dest(&op1, &op2);
	answer = *op2 = *op1;
	psw.nbit = (*op2 < 0 ? 1 : 0);
	psw.zbit = (*op2 == 0 ? 1 : 0);
	psw.vbit = 0;
}


movb() {
	int     *op1;
	int     *op2;
	char	*byte_1, *byte_2;

	two_op = byte_op = TRUE;
	get_source_dest(&op1, &op2);
	byte_1 = (char *)op1;
	byte_2 = (char *)op2;
	if (usehi_1)
		++byte_1;
	if (usehi_2)
		++byte_2;
	answer = *byte_2 = *byte_1;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
}


mul() {
	int     *dd;
	long    int     result;

	two_op = TRUE;
	fhighreg();
	highmode = 0;
	get_destination(&dd);
	result = (long)*dd * (long)r[highreg];
	answer = (int)result;
	if (!(highreg % 2)) {
		r[highreg + 1] = result & 0177777;
		result = result >> 16;
		r[highreg] = result & 0177777;
	}
	else
		r[highreg] = result & 0177777;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
	if (result >= 32767L || result < -32768L)
		psw.cbit = 1;
	else
		psw.cbit = 0;
}


neg() {
	int	*dd;

	get_destination(&dd);
	answer = *dd = -(*dd);
	psw.nbit =  (answer < 0 ? 1 : 0);
	if (answer == 0) {
		psw.zbit = 1;
		psw.cbit = 0;
	}
	else
		psw.zbit = 0;
	psw.vbit = (answer == 0100000 ? 1 : 0);
}


negb() {
	int	*dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	answer = *byte_1 = -(*byte_1);
	psw.nbit =  (answer < 0 ? 1 : 0);
	if (answer == 0) {
		psw.zbit = 1;
		psw.cbit = 0;
	}
	else
		psw.zbit = 0;
	psw.vbit = 0;
}


rol() {
	int	*dd;
	int	savebit;

	get_destination(&dd);
	if ((*dd & 0100000))
		savebit = 1;
	else
		savebit = 0;
	*dd = *dd << 1;
	*dd &= 0177776;
	answer = *dd |= psw.cbit;	
	if (savebit)
		psw.cbit = 1;
	else
		psw.cbit = 0;
	if (*dd < 0)
		psw.nbit = 1;
	else
		psw.nbit = 0;
	if (*dd == 0)
		psw.zbit = 1;
	else
		psw.zbit = 0;
	if (psw.cbit ^ psw.nbit)
		psw.vbit = 1;
	else
		psw.vbit = 0;
}


rolb() {
	/* NOT CURRENTLY DEFINED */
}


ror() {
	int     *dd;
	int	savebit;

	get_destination(&dd);
	savebit = *dd & 000001;
	*dd = *dd >> 1;
	if (psw.cbit) {
		psw.nbit = 1;
		*dd |= 0100000;
	}
	else {
		*dd &= 077777;
		psw.nbit = 0;
	}
	answer = *dd;
	psw.cbit = savebit;
	if (*dd == 0)
		psw.zbit = 1;
	else
		psw.zbit = 0;
	if (psw.cbit ^ psw.nbit)
		psw.vbit = 1;
	else
		psw.vbit = 0;
}


rorb() {
	/* NOT CURRENTLY DEFINED */
}


rts() {
	findlow();
	r[7] = r[lowreg];
	r[lowreg] = sys_stack[r[6]++];
	answer = r[7];
}

sbc() {
	int	*dd;
	int	temp;

	get_destination(&dd);
	temp = *dd;
	answer = *dd -= psw.cbit;
	if (*dd == 0100000)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	if (!temp && psw.cbit)
		psw.cbit = 0;
	if (*dd < 0)
		psw.nbit = 1;
	else
		psw.nbit = 0;
	if (*dd == 0)
		psw.zbit = 1;
	else
		psw.zbit = 0;
}


sbcb() {
	int     *dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	if (psw.cbit && !(*byte_1))
		psw.cbit = 0;
	answer = *byte_1 -= psw.cbit;
	if (answer < 0)
		psw.nbit = 1;
	else
		psw.nbit = 0;
	if (!(*byte_1))
		psw.zbit = 1;
	else
		psw.zbit = 0;
	psw.vbit = 0;
}


scc() {
	no_op = TRUE;
	answer = psw.nbit = psw.zbit = psw.vbit = psw.cbit = 1;
}


sec() {
	no_op = TRUE;
	answer = psw.cbit = 1;
}


sen() {
	no_op = TRUE;
	answer = psw.nbit = 1;
}


sev() {
	no_op = TRUE;
	answer = psw.vbit = 1;
}


sez() {
	no_op = TRUE;
	answer = psw.zbit = 1;
}


sob() {
	int     offset;

	fhighreg();
	lowmode = highmode = 0;
	lowreg = highreg;
	--r[highreg];
	if (r[highreg] != 0) {
		offset = ir & 077;
		r[7] -= offset * 2;
		answer = r[highreg];
	}
}


sub() {
	int     *op1, *op2;
	long    int     result;

	two_op = TRUE;
	get_source_dest(&op1, &op2);
	result = (long)*op2 - (long)*op1;
	answer = *op2 -= *op1;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	if (result > 32767L || result < -32768L)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	if (result & 0277777)
		psw.cbit = 0;

}


swab() {
	int	*dd;
	char	*byte_1, *byte_2, *byte_3;
	int	savebyte;

	get_destination(&dd);
	byte_1 = (char *)dd;
	byte_2 = byte_1 + 1;
	byte_3 = (char *)&savebyte;
	*byte_3 = *byte_1;
	*byte_1 = *byte_2;
	*byte_2 = *byte_3;
	answer = *dd;
	psw.nbit = (*byte_1 & 0200 ? 1 : 0);
	psw.zbit = (!(*byte_1) ? 1 : 0);
	psw.vbit = psw.cbit = 0;
}


sxt() {
	int     *dd;

	get_destination(&dd);
	if (psw.nbit)
		*dd = -1;
	else
		*dd = 0;
	answer = *dd;
}


tst() {
	int     *dd;

	get_destination(&dd);
	answer = *dd;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = psw.cbit = 0;
}


tstb() {
	int     *dd;
	char    *byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	answer = *byte_1;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = psw.cbit = 0;
}


xor() {
	int     *dd;

	two_op = TRUE;
	fhighreg();
	highmode = 0;
	findlow();
	get_destination(&dd);
	answer = *dd ^= r[highreg];
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
}
