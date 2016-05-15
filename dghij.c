# include "vm.h"

dec() {
	int	*dd;

	get_destination(&dd);
	psw.vbit = (*dd == 0100000 ? 1 : 0);
	answer = --(*dd);
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
}

decb() {
	int	*dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	answer = --(*byte_1);
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = (answer == 0777 ? 1 : 0);
}

div() {
	int	*dd;
	long	int	temp, result;

	two_op = TRUE;
	fhighreg();
	highmode = 0;
	get_destination(&dd);
	if ((*dd == 0) || (abs(r[highreg]) > abs(*dd))) {
		psw.vbit = 1;
		if (*dd == 0)
			psw.cbit = 1;
		else
			psw.cbit = 0;
	}
	else {
		result = (long)r[highreg];
		result = result << 16;
		temp = ((long)r[highreg] & 00177777);
		result |= temp;
		r[highreg + 1] = result % (long)*dd;
		r[highreg] = result / (long)*dd;
		psw.nbit = (r[highreg] < 0 ? 1 : 0);
		psw.zbit = (r[highreg] == 0 ? 1 : 0);
	}
	answer = r[highreg];
}

halt() {
	no_op = TRUE;
}

inc() {
	int	*dd;

	get_destination(&dd);
	psw.vbit = (*dd == 077777 ? 1 : 0);
	answer = ++(*dd);
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
}

incb() {
	int	*dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	answer = ++(*byte_1);
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = (answer == 0100 ? 1 : 0);
}

jmp() {
	int	dd;

	a_branch = branched = no_op = TRUE;
	findlow();
	findindex(&dd, lowmode, lowreg);
	r[7] = dd * 2;
}

jsr() {
	int	tmp;

	two_op = TRUE;
	dst = r[7] / 2;
	fhighreg();
	highmode = 0;
	findlow();
	findindex(&tmp, lowmode, lowreg);
	--r[6];
	sys_stack[r[6]] = r[highreg];
	r[highreg] = r[7];
	r[7] = tmp * 2;
	answer = r[7];
}
