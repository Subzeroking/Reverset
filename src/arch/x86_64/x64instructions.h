#ifndef _X64_INSTRUCTIONS_H
#define _X64_INSTRUCTIONS_H

typedef struct x64_instruction {
	char * mnemonic;
	union {
		struct {
			char * op1;
			char * op2;
			char * op3;
		};
		char * op[3];
	};
} x64_instruction;

enum x64_Addressing_Mode {
	X64_INVALID_MODE,
	X64_DIRECT_ADDRESSING = 'A',//Direct addressing. Address of operand is encoded in instruction.
	X64_FPU_REGISTER = 'B',	//The Reg field of Modrm selects a x87 fpu stack register
	X64_FPU_MODRM	= 'H',	//The modrm byte specifies either a x87 fpu stack register or memory address
	X64_CONTROL_REG = 'C',	//Reg field of modrm selects control register
	X64_DEBUG_REG = 'D',	//Reg field of modrm selects debug register
	X64_MODRM = 'E',		//Modrm byte
	X64_FLAGS_REG = 'F',	//Flags register
	X64_REG = 'G',			//Register field holds register
	X64_IMM = 'I',			//Immediate data
	X64_REL = 'J',			//RelatIde offset
	X64_MEM = 'M',			//Modrm forced to refer to memory
	X64_MOFFSET = 'O',		//No modrm byte. Offset is coded in the instruction
	X64_MOD_REG = 'R',		//Mod firled of modrm refers to general register
	X64_SEG_REG = 'S',		//The register field of the modrm byte codes for a seg reg
	X64_TEST_REG = 'T',		//Reg field of modrm byte codes for a test register
	X64_DSSI_MEM = 'X',		//Memory addressed by DS:SI
	X64_ESDI_MEM = 'Y'		//Memory addressed by ES:DI
};

enum x64_Operand_Size {
	X64_INVALID_SIZE,
	X64_TWO_WORD = 'a', //Two one word operands or two double word operands depending on operand size attribute
	X64_BYTE	 = 'b', //Byte 
	X64_BWORD	 = 'c', //Byte or word depending on operand size
	X64_DWORD	 = 'd', //Double word 
	X64_PTR		 = 'p', //32 or 48 bit pointer depending on operand size
	X64_PDESC	 = 's',	//6 byte pseudo descriptor
	X64_WDWORD	 = 'v',	//Word dword or qword depending on operand size and rex
	X64_WORD 	 = 'w',	//Word
	X64_QWORD 	 = 'q'	//Word
};

//Follows above operand encoding, unless the first character is in lowercase then the operand becomes that
static const x64_instruction x64_instruction_table[] = {
	//0
	{"add", "Eb", "Gb", ""},
	{"add", "Ev", "Gv", ""},
	{"add", "Gb", "Eb", ""},
	{"add", "Gv", "Ev", ""},
	{"add", "al", "Ib", ""},
	{"add", "eax", "Id", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"or", "Eb", "Gb", ""},
	{"or", "Ev", "Gv", ""},
	{"or", "Gb", "Eb", ""},
	{"or", "Gv", "Ev", ""},
	{"or", "al", "Ib", ""},
	{"or", "eax", "Id", ""},
	{"", "", "", ""},
	{"",	"", 	"", ""},//2 byte escape
	//1
	{"adc", "Eb", "Gb", ""},
	{"adc", "Ev", "Gv", ""},
	{"adc", "Gb", "Eb", ""},
	{"adc", "Gv", "Ev", ""},
	{"adc", "al", "Ib", ""},
	{"adc", "eax", "Id", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"sbb", "Eb", "Gb", ""},
	{"sbb", "Ev", "Gv", ""},
	{"sbb", "Gb", "Eb", ""},
	{"sbb", "Gv", "Ev", ""},
	{"sbb", "al", "Ib", ""},
	{"sbb", "eax", "Id", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//2
	{"and", "Eb", "Gb", ""},
	{"and", "Ev", "Gv", ""},
	{"and", "Gb", "Eb", ""},
	{"and", "Gv", "Ev", ""},
	{"and", "al", "Ib", ""},
	{"and", "eax", "Id", ""},
	{"", "", "", ""},	//Es segment register override
	{"", "", "", ""},
	{"sub", "Eb", "Gb", ""},
	{"sub", "Ev", "Gv", ""},
	{"sub", "Gb", "Eb", ""},
	{"sub", "Gv", "Ev", ""},
	{"sub", "al", "Ib", ""},
	{"sub", "rax", "Id", ""},
	{"", "", "", ""}, //Cs segment register override
	{"", "", "", ""},
	//3
	{"xor", "Eb", "Gb", ""},
	{"xor", "Ev", "Gv", ""},
	{"xor", "Gb", "Eb", ""},
	{"xor", "Gv", "Ev", ""},
	{"xor", "al", "Ib", ""},
	{"xor", "eax", "Id", ""},	
	{"", "", "", ""}, //SS segment register override
	{"", "", "", ""},
	{"cmp", "Eb", "Gb", ""},
	{"cmp", "Ev", "Gv", ""},
	{"cmp", "Gb", "Eb", ""},
	{"cmp", "Gv", "Ev", ""},
	{"cmp", "al", "Ib", ""},
	{"cmp", "rax", "Id", ""},
	{"", "", "", ""}, //DS segment register override
	{"", "", "", ""},
	//4
	{"", "", "", ""},	//Rex 	 
	{"", "", "", ""},	//Rex.b
	{"", "", "", ""},	//Rex.x
	{"", "", "", ""},	//Rex.xb
	{"", "", "", ""},	//Rex.r
	{"", "", "", ""},	//Rex.rb
	{"", "", "", ""},	//Rex.rx
	{"", "", "", ""},	//Rex.rxb
	{"", "", "", ""},	//rex.w
	{"", "", "", ""},	//Rex.wb
	{"", "", "", ""},	//Rex.wx
	{"", "", "", ""},	//Rex.wxb
	{"", "", "", ""},	//Rex.wr
	{"", "", "", ""},	//Rex.wrb
	{"", "", "", ""},	//Rex.wrx
	{"", "", "", ""},	//Rex.wrxb
	//5
	{"push", "rax", "", ""},
	{"push", "rcx", "", ""},
	{"push", "rdx", "", ""},
	{"push", "rbx", "", ""},
	{"push", "rsp", "", ""},
	{"push", "rbp", "", ""},
	{"push", "rsi", "", ""},
	{"push", "rdi", "", ""},
	{"pop", "rax", "", ""},
	{"pop", "rcx", "", ""},
	{"pop", "rdx", "", ""},
	{"pop", "rbx", "", ""},
	{"pop", "rsp", "", ""},
	{"pop", "rbp", "", ""},
	{"pop", "rsi", "", ""},
	{"pop", "rdi", "", ""},
	//6
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"movsxd", "Gv", "Ed", ""},
	{"", "", "", ""}, //FS Segment register override
	{"", "", "", ""}, //GS Segment register override
	{"", "", "", ""}, //Operand size override
	{"", "", "", ""}, //Address size override
	{"push", "Id", "", ""},
	{"imul", "Gv", "Ev", "Id"},
	{"push", "Ib", "", ""},
	{"imul", "Gv", "Ev", "Id"},
	{"insb", "Yb", "dx", ""},
	{"insw", "Yb", "dx", ""},
	{"outsb", "Dx", "Xb", ""},
	{"outsw", "dx", "Xv", ""},
	//7
	{"jo", "Jb", "", ""},
	{"jno", "Jb", "", ""},
	{"jb", "Jb", "", ""},
	{"jnb", "Jb", "", ""},
	{"jz", "Jb", "", ""},
	{"jnz", "Jb", "", ""},
	{"jbe", "Jb", "", ""},
	{"jnbe", "Jb", "", ""},
	{"js", "Jb", "", ""},
	{"jns", "Jb", "", ""},
	{"jp", "Jb", "", ""},
	{"jnp", "Jb", "", ""},
	{"jl", "Jb", "", ""},
	{"jge", "Jb", "", ""},
	{"jle", "Jb", "", ""},
	{"jg", "Jb", "", ""},
	//8
	{"grp1d", "Eb", "Ib", ""},
	{"grp1d", "Ev", "Id", ""},
	{"", "", "", ""},
	{"grp1d", "Ev", "Ib", ""},
	{"test", "Eb", "Gb", ""},
	{"test", "Ev", "Gv", ""},
	{"xchg", "Eb", "Gb", ""},
	{"xchg", "Ev", "Gv", ""},
	{"mov", "Eb", "Gb", ""},
	{"mov", "Ev", "Gv", ""},
	{"mov", "Gb", "Eb", ""},
	{"mov", "Gv", "Ev", ""},
	{"mov", "Ew", "Sw", ""},
	{"lea", "Gv", "M", ""},	//Change to Gv, M
	{"mov", "Sw", "Ew", ""},
	{"pop", "Ev", "", ""},
	//9
	{"nop", "", "", ""},	//Could also be xchg eax, eax
	{"xchg", "rcx", "rax", ""},
	{"xchg", "rdx", "rax", ""},
	{"xchg", "rbx", "rax", ""},
	{"xchg", "rsp", "rax", ""},
	{"xchg", "rbp", "rax", ""},
	{"xchg", "rsi", "rax", ""},
	{"xchg", "rdi", "rax", ""},
	{"cbw", "", "", ""},
	{"cwd", "", "", ""},
	{"", "", "", ""},
	{"wait", "", "", ""},
	{"pushf", "Fv", "", ""},
	{"popf", "Fv", "", ""},
	{"sahf", "", "", ""},
	{"lahf", "", "", ""},
	//A
	{"mov", "al", "Ob", ""},
	{"mov", "eax", "Ov", ""},
	{"mov", "Ob", "al", ""},
	{"mov", "Ov", "eax", ""},
	{"movsb", "Xb", "Yb", ""},
	{"movsw", "Xv", "Yv", ""},
	{"cmpsb", "Xb", "Yb", ""},
	{"cmpsw", "Xv", "Yv", ""},
	{"test", "al", "Ib", ""},
	{"test", "eax", "Id", ""},
	{"stosb", "Yb", "al", ""},
	{"stosw", "Yv", "eax", ""},
	{"lodsb", "al", "Xb", ""},
	{"lodsw", "eax", "Xv", ""},
	{"scasb", "al", "Xb", ""},
	{"scasw", "eax", "Xv", ""},
	//B
	{"mov", "al", "Ib", ""},
	{"mov", "cl", "Ib", ""},
	{"mov", "dl", "Ib", ""},
	{"mov", "bl", "Ib", ""},
	{"mov", "ah", "Ib", ""},
	{"mov", "ch", "Ib", ""},
	{"mov", "dh", "Ib", ""},
	{"mov", "bh", "Ib", ""},
	{"mov", "eax", "Id", ""},
	{"mov", "ecx", "Id", ""},
	{"mov", "edx", "Id", ""},
	{"mov", "ebx", "Id", ""},
	{"mov", "esp", "Id", ""},
	{"mov", "ebp", "Id", ""},
	{"mov", "esi", "Id", ""},
	{"mov", "edi", "Id", ""},
	//C
	{"grp2d", "Eb", "Ib", ""},
	{"grp2d", "Ev", "Ib", ""},
	{"ret", "Iw", "", ""},
	{"ret", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"mov", "Eb", "Ib", ""},
	{"mov", "Ev", "Id", ""},
	{"enter", "Iw", "Ib", ""},
	{"leave", "", "", ""},
	{"retf", "Iw", "", ""},
	{"retf", "", "", ""},
	{"int", "3", "", ""},
	{"int", "Ib", "", ""},
	{"into", "", "", ""},
	{"iret", "", "", ""},
	//D
	{"grp2d", "Eb", "1", ""},
	{"grp2d", "Ev", "1", ""},
	{"grp2d", "Eb", "cl", ""},
	{"grp2d", "Ev", "cl", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""}, //Not used
	{"xlat", "", "", ""},
	/*Floating Point Instruction Set*/
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	/*End Of Floating Point Instruction Set*/
	//E
	{"loopne", "Jb", "", ""},
	{"loope", "Jb", "", ""},
	{"loop", "Jb", "", ""},
	{"jcxz", "Jb", "", ""},
	{"in", "al", "Ib", ""},
	{"in", "eaxa", "Ib", ""},
	{"out", "Ib", "al", ""},
	{"out", "Ib", "eax", ""},
	{"call", "Av", "", ""},
	{"jmp", "Jv", "", ""},
	{"", "", "", ""},
	{"jmp", "Jb", "", ""},
	{"in", "al", "dx", ""},
	{"in", "eax", "dx", ""},
	{"out", "dx", "al", ""},
	{"out", "dx", "eax", ""},
	//F
	{"", "", "", ""},	//Lock prefix byte
	{"", "", "", ""},	//Not used
	{"", "", "", ""},	//Repne prefix byte
	{"", "", "", ""},	//Rep prefix byte
	{"hlt", "", "", ""},
	{"cmc", "", "", ""},
	{"grp3a", "", "", ""},
	{"grp3b", "", "", ""},
	{"clc", "", "", ""},
	{"stc", "", "", ""},
	{"cli", "", "", ""},
	{"sti", "", "", ""},
	{"cld", "", "", ""},
	{"std", "", "", ""},
	{"grp4d", "Eb", "", ""},
	{"grp5a", "", "", ""}

};

/*Currently only include standard x64 instruction set (no avx, sse, or fpu instructions) which is why its mostly empty*/
static const x64_instruction x64_instruction_extended_table[] = {
	//0
	{"grp6d", "Ew", "", ""},
	{"grp7a", "", "", ""},
	{"lar", "Gw", "Ew", ""},
	{"lsl", "Gv", "Ew", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"ctls", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//1
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"grp9d", "Ev", "", ""},
	//2
	{"mov", "Rd", "Cd", ""},
	{"mov", "Rd", "Dd", ""},
	{"mov", "Cd", "Rd", ""},
	{"mov", "Dd", "Rd", ""},
	{"mov", "Rd", "Td", ""},
	{"", "", "", ""},
	{"mov", "Td", "Rd", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//3
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//4
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//5
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//6
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//7
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//8
	{"jo", "Jv", "", ""},
	{"jno", "Jv", "", ""},
	{"jb", "Jv", "", ""},
	{"jnb", "Jv", "", ""},
	{"jz", "Jv", "", ""},
	{"jnz", "Jv", "", ""},
	{"jbe", "Jv", "", ""},
	{"jnbe", "Jv", "", ""},
	{"js", "Jv", "", ""},
	{"jns", "Jv", "", ""},
	{"jp", "Jv", "", ""},
	{"jnp", "Jv", "", ""},
	{"jl", "Jv", "", ""},
	{"jge", "Jv", "", ""},
	{"jle", "Jv", "", ""},
	{"jg", "Jv", "", ""},
	//9
	{"seto", "Eb", "", ""},
	{"setno", "Eb", "", ""},
	{"setb", "Eb", "", ""},
	{"setnb", "Eb", "", ""},
	{"setz", "Eb", "", ""},
	{"setnz", "Eb", "", ""},
	{"setbe", "Eb", "", ""},
	{"setnbe", "Eb", "", ""},
	{"sets", "Eb", "", ""},
	{"setns", "Eb", "", ""},
	{"setp", "Eb", "", ""},
	{"setnp", "Eb", "", ""},
	{"setl", "Eb", "", ""},
	{"setnl", "Eb", "", ""},
	{"setle", "Eb", "", ""},
	{"setnle", "Eb", "", ""},
	//A
	{"push", "fs", "", ""},
	{"pop", "fs", "", ""},
	{"", "", "", ""},
	{"bt", "Ev", "Gv", ""},
	{"shld", "Ev", "Gv", "Ib"},
	{"shld", "Ev", "Gv", "cl"},
	{"", "", "", ""},
	{"", "", "", ""},
	{"push", "gs", "", ""},
	{"pop", "gs", "", ""},
	{"", "", "", ""},
	{"bts", "Ev", "Gv", ""},
	{"shrd", "Ev", "Gv", "Ib"},
	{"shrd", "Ev", "Gv", "cl"},
	{"", "", "", ""},
	{"imul", "Gv", "Ev", ""},
	//B
	{"", "", "", ""},
	{"", "", "", ""},
	{"lss", "Mp", "", ""},
	{"btr", "Ev", "Gv", ""},
	{"lfs", "Mp", "", ""},
	{"lgs", "Mp", "", ""},
	{"movzx", "Gv", "Eb", ""},
	{"movzx", "Gv", "Ew", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"grp8d", "Ev", "Ib", ""},
	{"btc", "Ev", "Gv", ""},
	{"bsf", "Gv", "Ev", ""},
	{"bsr", "Gv", "Ev", ""},
	{"movsx", "Gv", "Eb", ""},
	{"movsx", "Gv", "Ew", ""},
	//C
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//D
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//E
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	//F
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},

};

//Opcode extension groups
static const x64_instruction x64_grp1[] = {
	{"add", "", "", ""},
	{"or", "", "", ""},
	{"adc", "", "", ""},
	{"sbb", "", "", ""},
	{"and", "", "", ""},
	{"sub", "", "", ""},
	{"xor", "", "", ""},
	{"cmp", "", "", ""}
};

static const x64_instruction x64_grp2[] = {
	{"rol", "", "", ""},
	{"ror", "", "", ""},
	{"rcl", "", "", ""},
	{"rcr", "", "", ""},
	{"shl", "", "", ""},
	{"shr", "", "", ""},
	{"sal", "", "", ""},
	{"sar", "", "", ""}
};

static const x64_instruction x64_grp3[] = {
	{"test", "Eb", "Ib", ""},
	{"test", "Eb", "Ib", ""},
	{"not", "Eb", "", ""},
	{"neg", "Eb", "", ""},
	{"mul", "al", "Eb", ""},
	{"imul", "al", "Eb", ""},
	{"dId", "al", "Eb", ""},
	{"idId", "al", "Eb", ""},
	//
	{"test", "Ev", "Id", ""},
	{"test", "Ev", "Id", ""},
	{"not", "Ev", "", ""},
	{"neg", "Ev", "", ""},
	{"mul", "eax", "Ev", ""},
	{"imul", "eax", "Ev", ""},
	{"dId", "eax", "Ev", ""},
	{"idId", "eax", "Ev", ""}
};

static const x64_instruction x64_grp4[] = {
	{"inc", "", "", ""},
	{"dec", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""}
};

static const x64_instruction x64_grp5[] = {
	{"inc", "Ev", "", ""},
	{"dec", "Ev", "", ""},
	{"call", "Ev", "", ""},
	{"callf", "", "", ""},//don't know how to decode this yet
	{"jmp", "Eq", "", ""},
	{"jmp", "Ep", "", ""},
	{"push", "Eq", "", ""},
	{"", "", "", ""}
};

static const x64_instruction x64_grp6[] = {
	{"sldr", "", "", ""},
	{"str", "", "", ""},
	{"lldt", "", "", ""},
	{"ltr", "", "", ""},
	{"verr", "", "", ""},
	{"verw", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""}
};

static const x64_instruction x64_grp7[] = {
	{"sgdt", "Ms", "", ""},
	{"sidt", "Ms", "", ""},
	{"ldgt", "Ms", "", ""},
	{"lidt", "Ms", "", ""},
	{"smsw", "Ew", "", ""},
	{"", "", "", ""},
	{"lmsw", "Ew", "", ""},
	{"", "", "", ""}
};

static const x64_instruction x64_grp8[] = {
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"", "", "", ""},
	{"bt", "", "", ""},
	{"bts", "", "", ""},
	{"btr", "", "", ""},
	{"btc", "", "", ""}
};

static const x64_instruction x64_grp9[] = {
	{"nop", "", "", ""},
	{"nop", "", "", ""},
	{"nop", "", "", ""},
	{"nop", "", "", ""},
	{"nop", "", "", ""},
	{"nop", "", "", ""},
	{"nop", "", "", ""},
	{"nop", "", "", ""}
};

static const x64_instruction* x64_groups[10] = {
	x64_grp1, x64_grp2, x64_grp3, x64_grp4, x64_grp5, x64_grp6, x64_grp7, x64_grp8, x64_grp9
};

#endif