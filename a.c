# include "vm.h"

adc() {
	int	*dd;
	int	temp;

	get_destination(&dd);
	temp = *dd;
	*dd += psw.cbit;
	if (temp == 077777 && psw.cbit)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	if (temp == 0177777 && psw.cbit)
		psw.cbit = 1;
	else
		psw.cbit = 0;
	psw.nbit = (*dd < 0 ? 1 : 0);
	psw.zbit = (*dd == 0 ? 1 : 0);
	answer = *dd;	 
}


adcb() {
	int	*dd;
	char	*byte_1;

	byte_op = TRUE;
	get_destination(&dd);
	byte_1 = (char *)dd;
	if (use_high)
		++byte_1;
	if ((psw.cbit == 1) && (*byte_1 == -1))
		psw.cbit = 1;
	*byte_1 += psw.cbit;
	answer = *byte_1;
	psw.nbit = (*byte_1 < 0 ? 1 : 0);
	psw.zbit = (*byte_1 == 0 ? 1 : 0);
	psw.vbit = 0;
}


add() {
	int     *op1, *op2;
	long    int     result;

	two_op = TRUE;
	get_source_dest(&op1, &op2);
	result = (long)*op1 - (long)*op2;
	answer = *op2;
	psw.nbit = (*op2 < 0 ? 1 : 0);
	psw.zbit = (*op2 == 0 ? 1 : 0);
	if (result > 32767L || result < -32768L)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	psw.cbit = (result & 0277777L ? 1 : 0);
}


ash() {
	int	*dd;
	int	i, tempreg, numshift;

	two_op = TRUE;
	fhighreg();
	highmode = 0;
	get_destination(&dd);
	tempreg = r[highreg];
	if (*dd < 0)
		numshift = -(*dd);
	else
		numshift = *dd;
	for (i = 1; i <= numshift; ++i) {
		if (*dd < 0) {
			psw.cbit = r[highreg] & 000001;
			r[highreg] = r[highreg] >> 1;
		}
		else {
			psw.cbit = r[highreg] & 0100000;
			r[highreg] = r[highreg] << 1;
		}
	}
	answer = r[highreg];
	if (answer < 0 && tempreg > 0)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	if (answer > 0 && tempreg < 0)
		psw.vbit = 1;
	else
		psw.vbit = 0;
	if (!answer)
		psw.zbit = 1;
	else
		psw.zbit = 0;
	if (answer < 0)
		psw.nbit = 1;
	else
		psw.nbit = 0;
}


asl() {
	int	*dd;

	get_destination(&dd);
	if ((*dd & 0100000))
		psw.cbit = 1;
	else
		psw.cbit = 0;
	answer = *dd = *dd << 1;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	if ((psw.nbit ^ psw.cbit))
		psw.vbit = 1;
	else
		psw.vbit = 0;
}


asr() {
	int	*dd;

	get_destination(&dd);
	psw.cbit = *dd & 000001;
	*dd = *dd >> 1;
	answer = *dd;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = psw.nbit ^ psw.cbit;
}


asrb() {
	/* NOT CURRENTLY DEFINED */
}


aslb() {
	/* NOT CURRENTLY DEFINED */
}
