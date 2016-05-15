# include "vm.h"

int	memory[MEMSIZE];
int	sys_stack[STACK_SIZE];
int	nummodes = 24;

int	memuse;
int	r[8];
int	last_pc;
int	ir;
int	src;
int	dst;
int	instruction;
int	memword;

BOOLEAN	trace_on = FALSE;
BOOLEAN	branched = FALSE;
BOOLEAN a_branch = FALSE;
BOOLEAN two_op = FALSE;
BOOLEAN no_op = FALSE;
BOOLEAN	byte_op = FALSE;
BOOLEAN use_high = FALSE;
BOOLEAN	usehi_1 = FALSE;
BOOLEAN	usehi_2 = FALSE;

int	answer;
char	*opstring;

int	lowmode;
int	highmode;
int	lowreg;
int	highreg;

int	s_time = 0;
int	f_time = 0;
float	t_final;
int	num_instr = 1;

struct	tbuffer	buffer;

struct	print	print[] = {
	{"register"},
	{"register deferred"},
	{"autoincrement"},
	{"autoincrement deferred"},
	{"autodecrement"},
	{"autodecrement deferred"},
	{"index"},
	{"index deferred"},
	{"immediate"},
	{"absolute"},
	{"relative"},
	{"relative deferred"}
};

struct	psw	psw;

struct	opcodetbl	opcodetbl[] = {

	{"mov ", 0170000, 0010000, mov }, {"cmp ", 0170000, 0020000, cmp },
	{"add ", 0170000, 0060000, add }, {"sub ", 0170000, 0160000, sub },
	{"bic ", 0170000, 0040000, bic }, {"bis ", 0170000, 0050000, bis },
	{"movb", 0170000, 0110000, movb}, {"cmpb", 0170000, 0120000, cmpb},
	{"bicb", 0170000, 0140000, bicb}, {"bisb", 0170000, 0150000, bisb},
	{"clr ", 0177700, 0005000, clr }, {"com ", 0177700, 0005100, com },
	{"inc ", 0177700, 0005200, inc }, {"dec ", 0177700, 0005300, dec },
	{"neg ", 0177700, 0005400, neg }, {"adc ", 0177700, 0005500, adc },
	{"sbc ", 0177700, 0005600, sbc }, {"tst ", 0177700, 0005700, tst },
	{"ror ", 0177700, 0006000, ror }, {"rol ", 0177700, 0006100, rol },
	{"asr ", 0177700, 0006200, asr }, {"asl ", 0177700, 0006300, asl },
	{"sxt ", 0177700, 0006700, sxt }, {"jmp ", 0177700, 0000100, jmp },
	{"swab", 0177700, 0000300, swab},

	{"clrb", 0177700, 0105000, clrb}, {"comb", 0177700, 0105100, comb},
	{"incb", 0177700, 0105200, incb}, {"decb", 0177700, 0105300, decb},
	{"negb", 0177700, 0105400, negb}, {"adcb", 0177700, 0105500, adcb},
	{"sbcb", 0177700, 0105600, sbcb}, {"tstb", 0177700, 0105700, tstb},
	{"rorb", 0177700, 0106000, rorb}, {"rolb", 0177700, 0106100, rolb},
	{"asrb", 0177700, 0106200, asrb}, {"dump", 0177700, 0106300, dump},

	{"xor ", 0177000, 0074000, xor }, {"mul ", 0177000, 0070000, mul },
	{"div ", 0177000, 0071000, div }, {"ash ", 0177000, 0072000, ash },
	{"jsr ", 0177000, 0004000, jsr }, {"rts ", 0177770, 0000200, rts },

	{"br  ", 0177400, 0000400, br  }, {"bne ", 0177400, 0001000, bne },
	{"beq ", 0177400, 0001400, beq }, {"bpl ", 0177400, 0100000, bpl },
	{"bmi ", 0177400, 0100400, bmi }, {"bvc ", 0177400, 0102000, bvc },
	{"bvs ", 0177400, 0102400, bvs }, {"bcs ", 0177400, 0103400, bcs },
	{"bge ", 0177400, 0002000, bge }, {"blt ", 0177400, 0002400, blt },
	{"bgt ", 0177400, 0003000, bgt }, {"ble ", 0177400, 0003400, ble },
	{"bhi ", 0177400, 0101000, bhi }, {"blos", 0177400, 0101400, blos},
	{"bhis", 0177400, 0103000, bhis}, {"blo ", 0177400, 0103400, blo },

	{"halt", 0177777, 0000000, halt}, {"clc ", 0177777, 0000241, clc },
	{"clv ", 0177777, 0000242, clv }, {"clz ", 0177777, 0000244, clz },
	{"cln ", 0177777, 0000250, cln }, {"ccc ", 0177777, 0000257, ccc },
	{"sec ", 0177777, 0000261, sec }, {"sev ", 0177777, 0000262, sev },
	{"sez ", 0177777, 0000263, sez }, {"sen ", 0177777, 0000264, sen },
	{"scc ", 0177777, 0000277, scc }, {"sob ", 0177000, 0077000, sob }
};

struct	modetbl	modetbl[] = {	/* Low order register formats */

	{0000070, 0000000, REG},	{0000070, 0000010, REGDEF},
	{0000070, 0000020, AUTOI},	{0000070, 0000030, AUTOIDEF},
	{0000070, 0000040, AUTOD},	{0000070, 0000050, AUTODDEF},
	{0000070, 0000060, INDEX},      {0000070, 0000070, INDEXDEF},
	{0000077, 0000027, IMMEDIATE},	{0000077, 0000037, ABSOLUTE},
	{0000077, 0000067, RELATIVE},

		/* High order register formats */

	{0007000, 0000000, REG},	{0007000, 0001000, REGDEF},
	{0007000, 0002000, AUTOI},	{0007000, 0003000, AUTOIDEF},
	{0007000, 0004000, AUTOD},	{0007000, 0005000, AUTODDEF},
	{0007000, 0006000, INDEX},	{0007000, 0007000, INDEXDEF},
	{0007700, 0002700, IMMEDIATE},	{0007700, 0003700, ABSOLUTE},
	{0007700, 0006700, RELATIVE},	{0007700, 0007700, RELDEF}
};

struct	regtbl	regtbl[] = {
	{0000007, 0000000, 0},		{0000007, 0000001, 1},
	{0000007, 0000002, 2},		{0000007, 0000003, 3},
	{0000007, 0000004, 4},		{0000007, 0000005, 5},
	{0000007, 0000006, 6},		{0000007, 0000007, 7},

	{0000700, 0000000, 0},		{0000700, 0000100, 1},
	{0000700, 0000200, 2},          {0000700, 0000300, 3},
	{0000700, 0000400, 4},          {0000700, 0000500, 5},
	{0000700, 0000600, 6},          {0000700, 0000700, 7}
};

main(argc, argv)
int     argc;
char    **argv;
{
	/* times(&buffer); */
	s_time = buffer.proc_user_time;
	if (argc > 1) {
		if (!strcmp(argv[1], "-t"))
			trace_on = TRUE;
	}
	vminit();
	getoctal();
	ifetch();
	findopcd();
	if (trace_on)
		trace_head();
	while (opcodetbl[instruction].opcode != HALT) {
		exopcode();
		byte_op = FALSE;
		if (trace_on)
			vmtrace();
		ifetch();
		findopcd();
	}
	no_op = TRUE;
	if (trace_on)
		vmtrace();
	write_time();
}

vminit() {
	r[6] = STACK_SIZE - 1;
}

write_time() {
	/* times(&buffer); */
	f_time = buffer.proc_user_time;
	t_final = (f_time - s_time) / 60.00;
	printf("\n%06d instructions executed in %5.3f seconds of CPU time.\n",
		num_instr, t_final);
	t_final = (float)t_final / (float)num_instr;
	printf("Average instruction execution time: %10.7f seconds.\n",
		t_final);
}

vmtrace() {
	char    *bstring;

	if (usehi_1)
		src = src * 2 + 1;
	else
		src = src * 2;
	if (usehi_2)
		dst = dst * 2 + 1;
	else
		dst = dst * 2;
	printf("|%06o|   %06o  |%4s|", last_pc, memword, opstring);
	if (two_op) {
		if ((highmode > 0) && (lowmode > 0)) {
			printf(" %o |%21s|%03o|***| %o |%21s|%03o|%06o",
				highreg, print[highmode].printmode, src,
				lowreg, print[lowmode].printmode,
				dst, answer);
		}
		else if ((highmode == 0) && (lowmode == 0)) {
			printf(" %o |%21s|---|***| %o |%21s|---|%06o",
				highreg, print[highmode].printmode,
				lowreg, print[lowmode].printmode,
				answer);
		}
		else if ((highmode == 0) && (lowmode > 0)) {
			printf(" %o |%21s|---|***| %o |%21s|%03o|%06o",
				highreg, print[highmode].printmode,
				lowreg, print[lowmode].printmode,
				dst, answer);
		}
		else if ((highmode > 0) && (lowmode == 0)) {
			printf(" %o |%21s|%03o|***| %o |%21s|---|%06o",
				highreg, print[highmode].printmode,
				src, lowreg,
				print[lowmode].printmode,
				answer);
		}
	}
	else if (no_op) {
		if (a_branch) {
			if (branched)
				bstring = "branch";
			else
				bstring = "contin";
			a_branch = branched = FALSE;
		}
		else
			bstring = "------";
		printf("---|---------------------|---|***|---|---------------------|---|%s",
			bstring);
	}
	else {
		if (lowmode > 0) {
			printf("---|---------------------|---|***| %o |%21s|%03o|%06o",
			lowreg, print[lowmode].printmode, dst, answer);
		}
		else {
			printf("---|---------------------|---|***| %o |%21s|---|%06o",
			lowreg, print[lowmode].printmode, answer);
		}
	}
	printf("|***|  %o  |  %o  |  %o  |  %o  |\n",
		psw.nbit, psw.zbit, psw.vbit, psw.cbit);
	two_op = FALSE;
	no_op = 0;
}

trace_head() {
	printf("+--------------------------------------- PDP-11/70 VIRTUAL MACHINE INSTRUCTION TRACE --------------------------------------+\n");
	printf("|      |           |    |   |                     |   |***|   |                     |   |      |***| processor status word |\n");
	printf("|  pc  |instruction| op |hi |        mode         |hi |***|low|        mode         |low|result|***|  n  |  z  |  v  |  c  |\n");
	printf("|      |  fetched  |code|reg|                     |mem|***|reg|                     |mem|      |***|     |     |     |     |\n");
	printf("+--------------------------------------------------------------------------------------------------------------------------+\n");
}
