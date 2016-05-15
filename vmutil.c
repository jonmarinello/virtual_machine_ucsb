# include "vm.h"

branch() {
	int     offset;

	branched = TRUE;
	offset = ir & 0377;
	if ((ir & 000200))
		offset -= 0400;
	answer = r[7] += offset * 2;
}


exopcode() {
	++num_instr;
	(* opcodetbl[instruction].fptr)();
}


fhighreg() {
	unsigned        i;

	for (i = 8; i < 16; ++i) {
		if ((ir & regtbl[i].regmask) == regtbl[i].regcode) {
			highreg = regtbl[i].regnum;
			return;
		}
	}
}


fhimode(start)
int     start;
{
	unsigned        i;

	for (i = start; i < nummodes; ++i) {
		if ((ir & modetbl[i].modemask) == modetbl[i].modecode) {
			highmode = modetbl[i].mode;
			return;
		}
	}
	printf("\n\007vm: can't find high register mode\n");
}


findhilow() {
	fhighreg();
	flowreg();
	if (highreg == 7)
		fhimode(/*20*/19);
	else
		fhimode(11);
	if (lowreg == 7)
		flowmode(8);
	else
		flowmode(0);
}


findindex(ea, modetype, ireg)
int     *ea;
int     modetype;
int     ireg;
{
	use_high = FALSE;
	switch (modetype) {
	case 1 :
		if (byte_op)
			if ((r[ireg] % 2) != 0)
				use_high = TRUE;
		*ea = r[ireg] / 2;
		break;
	case 2 :
		*ea = r[ireg] / 2;
		if (byte_op) {
			if ((r[ireg] % 2) != 0)
				use_high = TRUE;
			++r[ireg];
		}
		else
			r[ireg] += 2;
		break;
	case 3 :
		*ea = memory[r[ireg] / 2] / 2;
		r[ireg] += 2;
		break;
	case 4 :
		if (byte_op) {
			--r[ireg];
			if ((r[ireg] % 2) != 0)
				use_high = TRUE;
		}
		else
			r[ireg] -= 2;
		*ea = r[ireg] / 2;
		break;
	case 5 :
		r[ireg] -= 2;
		*ea = memory[r[ireg] / 2] / 2;
		break;
	case 6 :
		if (byte_op)
			if ((r[ireg] % 2) != 0)
				use_high = TRUE;
		*ea = (memory[r[7] / 2] + r[ireg] / 2) / 2;
		r[7] += 2;
		break;
	case 7 :
		*ea = memory[memory[r[7] / 2] / 2 + memory[r[ireg] / 2]];
		r[7] += 2;
		break;
	case 8 :
		*ea = r[7] / 2;
		r[7] += 2;
		break;
	case 9 :
		*ea = memory[r[7] / 2] / 2;
		r[7] += 2;
		break;
	case 10 :
		*ea = memory[r[7] / 2] / 2 + (r[7] / 2 + 1);
		r[7] += 2;
		break;
	case 11 :
		*ea = memory[memory[r[7] / 2] / 2 + (r[7] / 2 + 1)];
		r[7] += 2;
		break;
	}
}


findlow() {
	flowreg();
	if (lowreg == 7)
		flowmode(8);
	else
		flowmode(0);
}


findopcd() {
	unsigned	i;

	for (i = 0; i < NUM_OPS; ++i) {
		if ((ir & opcodetbl[i].opmask) ==
			opcodetbl[i].opcode) {
				instruction = i;
				opstring = opcodetbl[i].numonic;
				memword = ir;
				return;
		}
	}
	fprintf(stderr, "vm: unknown opcode %06o\n", ir);
}


flowmode(start)
int     start;
{
	unsigned        i;

	for (i = start; i < nummodes; ++i) {
		if ((ir & modetbl[i].modemask) == modetbl[i].modecode) {
			lowmode = modetbl[i].mode;
			return;
		}
	}
	printf("\n\007vm: can't low high register mode\n");
}


flowreg() {
	unsigned        i;

	for (i = 0; i < 16; ++i) {
		if ((ir & regtbl[i].regmask) == regtbl[i].regcode) {
			lowreg = regtbl[i].regnum;
			return;
		}
	}
}


getoctal() {
	char    buf[256];
	char    nmumonic[128];

	while (gets(buf)) {
		if (*buf == '#') {        /* skip comments */
#if DEBUGF
			printf("%s\n", buf);
#endif
			continue;
		}
		sscanf(buf, "%o", &memory[memuse]);
		strcpy(nmumonic, buf + 7);
#if DEBUGF
		printf("%06o\t%s\n", memory[memuse], nmumonic);
#endif
		++memuse;
		buf[7] = '\0';
	}
}


ifetch() {
	ir = memory[r[7] / 2];
	last_pc = r[7];
	r[7] += 2;
}


dump() {
	unsigned	i;
	no_op = TRUE;

	if (trace_on)
		printf("+--------------------------------------------------------------------------------------------------------------------------+\n");
	printf("============================================================ DUMP ==========================================================\n\n");
	printf("      pc = %06o", r[7]);
	printf("   sp = %06o", r[6]);
	printf("   r0 = %06o", r[0]);
	printf("   r1 = %06o", r[1]);
	printf("   r2 = %06o", r[2]);
	printf("   r3 = %06o", r[3]);
	printf("   r4 = %06o", r[4]);
	printf("   r5 = %06o\n\n", r[5]);
	printf("............................................................................................................................\n");
	for (i = 0; i < memuse; ++i) {
		if (!(i % 15))
			printf("\n");
		printf("  %06o", memory[i]);
	}
	printf("\n============================================================================================================================\n");
	if (trace_on)
		trace_head();
}

get_source_dest(source, destin)
int	**source;
int     **destin;
{
	findhilow();
	if (!highmode && !lowmode) {
		*source = &r[highreg];
		*destin = &r[lowreg];
	}
	else if (!highmode && lowmode > 0) {
		usehi_1 = FALSE;
		findindex(&dst, lowmode, lowreg);
		usehi_2 = use_high;
		*source = &r[highreg];
		*destin = &memory[dst];
	}
	else if (highmode > 0 && !lowmode) {
		usehi_2 = FALSE;
		findindex(&src, highmode, highreg);
		usehi_1 = use_high;
		*source = &memory[src];
		*destin = &r[lowreg];
	}
	else {
		findindex(&src, highmode, highreg);
		usehi_1 = use_high;
		findindex(&dst, lowmode, lowreg);
		usehi_2 = use_high;
		*source = &memory[src];
		*destin = &memory[dst];
	}
}

get_destination(destin)
int	**destin;
{
	findlow();
	if (!lowmode)
		*destin = &r[lowreg];
	else {
		findindex(&dst, lowmode, lowreg);
		usehi_2 = use_high;
		*destin = &memory[dst];
	}
}
