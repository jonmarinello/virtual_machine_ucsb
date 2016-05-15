# include <stdio.h>

# define DEBUGF         1       /* set debug flag for development */

# define BOOLEAN	int
# define TRUE		1
# define FALSE		0
# define MEMSIZE	542
# define STACK_SIZE	50
# define NUM_OPS	71
# define HALT		0000000
# ifndef EOF
# define EOF		-1
# endif

# define REG		0
# define REGDEF		1
# define AUTOI		2
# define AUTOIDEF	3
# define AUTOD		4
# define AUTODDEF	5
# define INDEX		6
# define INDEXDEF	7

# define IMMEDIATE	8
# define ABSOLUTE	9
# define RELATIVE	10
# define RELDEF		11

extern	int	memory[];
extern	int	sys_stack[];
extern	int	nummodes;

extern	int	memuse;
extern	int	r[];
extern	int	last_pc;
extern	int	ir;
extern	int	src;
extern	int	dst;
extern	int	instruction;
extern	int	memword;

extern	BOOLEAN	trace_on;
extern	BOOLEAN	branched;
extern	BOOLEAN a_branch;
extern	BOOLEAN two_op;
extern	BOOLEAN no_op;
extern	BOOLEAN	byte_op;
extern	BOOLEAN	use_high;
extern	BOOLEAN	usehi_1;
extern	BOOLEAN	usehi_2;

extern	int	answer;
extern	char	*opstring;

extern	int	lowmode;
extern	int	highmode;
extern	int	lowreg;
extern	int	highreg;

extern	int	s_time;
extern	int	f_time;
extern	float	t_final;
extern	int	num_instr;

struct	tbuffer	{
	int	proc_user_time;
	int	proc_system_time;
	int	child_user_time[2];
	int	child_system_time[2];
};

extern	struct	tbuffer	buffer;

struct	print	{
	char	*printmode;
};

extern	struct	print	print[];

struct	psw	{
	int	nbit;
	int	zbit;
	int	vbit;
	int	cbit;
};

extern	struct	psw	psw;

extern	int	mov(), cmp(), add(), sub(), bic(), bis(), movb(), cmpb(),
		bicb(), bisb(), clr(), com(), inc(), dec(), neg(),
		adc(), sbc(), tst(), ror(), rol(), asr(), asl(),
		sxt(), jmp(), swab(), clrb(), comb(), incb(), decb(), negb(),
		adcb(), sbcb(), tstb(), rorb(), rolb(), asrb(), dump(), xor(),
		mul(), div(), ash(), jsr(), rts(), br(), bne(), beq(),
		bpl(), bmi(), bvc(), bvs(), bcs(), bge(), blt(), bgt(),
		ble(), bhi(), blos(), bhis(), blo(), halt(), clc(), clv(),
		clz(), cln(), ccc(), sec(), sev(), sez(), sen(), scc(), sob();

struct	opcodetbl	{
	char	*numonic;
	int	opmask;
	int	opcode;
	int	(*fptr)();
};

extern	struct	opcodetbl	opcodetbl[];

struct	modetbl	{
	int	modemask;
	int	modecode;
	int	mode;
};

extern	struct	modetbl	modetbl[];

struct	regtbl	{
	int	regmask;
	int	regcode;
	int	regnum;
};

extern	struct	regtbl	regtbl[];
