# include "vm.h"

bcc() {			/* Not implemented in call table */
	if (!psw.cbit)
		branch();
	a_branch = no_op = TRUE;
}


bcs() {
	if (psw.cbit)
		branch();
	a_branch = no_op = TRUE;
}


beq() {
	if (psw.zbit)
		branch();
	a_branch = no_op = TRUE;
}


bge() {
	if (!(psw.nbit ^ psw.vbit))
		branch();
	a_branch = no_op = TRUE;
}


bgt() {
	if (!(psw.zbit | (psw.nbit ^ psw.vbit)))
		branch();
	a_branch = no_op = TRUE;
}


bhi() {
	if (!psw.cbit && !psw.zbit)
		branch();
	a_branch = no_op = TRUE;
}


bhis() {
	if (!psw.cbit)
		branch();
	a_branch = no_op = TRUE;
}


bic() {
	int	*op1, *op2;

	two_op = TRUE;
	get_source_dest(&op1, &op2);
	answer = *op2 = ~*op1 & *op2;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
}


bicb() {
	int	*op1, *op2;
	char	*byte_1, *byte_2;

	two_op = byte_op = TRUE;
	get_source_dest(&op1, &op2);
	byte_1 = (char *)op1;
	byte_2 = (char *)op2;
	if (usehi_1)
		++byte_1;
	if (usehi_2)
		++byte_2;
	*byte_2 = ~*byte_1 & *byte_2;
	answer = *byte_2;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
}


bis() {
	int	*op1, *op2;

	two_op = TRUE;
	get_source_dest(&op1, &op2);
	answer = *op2 = *op1 | *op2;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
}


bisb() {
	int	*op1, *op2;
	char	*byte_1, *byte_2;

	two_op = byte_op = TRUE;
	get_source_dest(&op1, &op2);
	byte_1 = (char *)op1;
	byte_2 = (char *)op2;
	if (usehi_1)
		++byte_1;
	if (usehi_2)
		++byte_2;
	*byte_2 = *byte_1 | *byte_2;
	answer = *byte_2;
	psw.nbit = (answer < 0 ? 1 : 0);
	psw.zbit = (answer == 0 ? 1 : 0);
	psw.vbit = 0;
}


ble() {
	if ((psw.zbit | (psw.nbit ^ psw.vbit)))
		branch();
	a_branch = no_op = TRUE;
}


blo() {
	if (psw.cbit)
		branch();
	a_branch = no_op = TRUE;
}


blos() {
	if (psw.cbit || psw.zbit)
		branch();
	a_branch = no_op = TRUE;
}


blt() {
	if ((psw.nbit ^ psw.vbit))
		branch();
	a_branch = no_op = TRUE;
}


bmi() {
	if (psw.nbit)
		branch();
	a_branch = no_op = TRUE;
}


bne() {
	if (!psw.zbit)
		branch();
	a_branch = no_op = TRUE;
}


bpl() {
	if (!psw.nbit)
		branch();
	a_branch = no_op = TRUE;
}


br() {
	branch();
	a_branch = no_op = TRUE;
}


bvc() {
	if (!psw.vbit)
		branch();
	a_branch = no_op = TRUE;
}


bvs() {
	if (psw.vbit)
		branch();
	a_branch = no_op = TRUE;
}
