#include "ranal.h"




void r_meta_analyze(r_analyzer * anal, r_disassembler * disassembler, r_file * file)
{
	if (file->arch == r_x86_64)
		r_meta_rip_resolve(disassembler, file);

	r_meta_calculate_branches(anal, disassembler);
	r_meta_reloc_resolve(disassembler, file);

	r_meta_symbol_replace(disassembler, file);
	r_meta_string_replace(disassembler, file);
}

void r_meta_calculate_branches(r_analyzer * anal, r_disassembler * disassembler)
{
	//Upon reanalysis assume instruction have changed so remove all branches
	if (anal->branches) free(anal->branches);
	anal->num_branches = 0;

	for (int j = 0; j < disassembler->num_instructions; j++) {
		r_disasm * disas = disassembler->instructions[j];
		if (disas->metadata->type == r_tcjump || disas->metadata->type == r_tujump) {
			//Check this branch doesnt already exist
			int found = -1;
			for (int i = 0; i < anal->num_branches; i++) {
				if (anal->branches[i].start == disas->address) {
					found = i;
					break;
				}
			}

			r_branch branch;
			branch.start = disas->address;
			branch.end = 0;
			branch.indirect = 0;
			branch.nested = 0;
			branch.conditional = disas->metadata->type == r_tujump;

			int status = 0;
			for (int i = 0; i < disas->num_operands; i++) {				
				branch.end = r_meta_get_address(disas->op[i], &status);
				if (status == 2) branch.indirect = 1;
				else if (!status) continue;
				break;
			}
			if (branch.end > branch.start) {
				branch.dir = 0;
			} else {
				branch.dir = 1;
				uint64_t tmp = branch.end;
				branch.end = branch.start;
				branch.start = tmp;
			}
			if (status != 0 && found == -1) {
				anal->num_branches++;
				if (anal->num_branches == 1) {
					anal->branches = malloc(sizeof(r_branch));
				} else {
					anal->branches = realloc(anal->branches, sizeof(r_branch) * anal->num_branches);
				}
				anal->branches[anal->num_branches-1] = branch;
			} else if (status != 0 && found < anal->num_branches) {
				anal->branches[found] = branch;
			}
		}
	}

	for (int i = 0; i < anal->num_branches; i++) {
		r_branch b = anal->branches[i];

		int bcase = 0;
		for (int j = 0; j < anal->num_branches; j++) {
			if (j==i) continue;

			r_branch b2 = anal->branches[j];
			bcase = 0;
			if ((b2.start > b.start) && (b2.end < b.end)) bcase = 1;
			else if ((b2.start > b.start) && (b2.start < b.end)) bcase = 2;
			else if ((b2.end > b.start) && (b2.end < b.end)) bcase = 3;

			switch (bcase) {
				case 1:
					anal->branches[i].nested++;
					break;
				case 2:
					anal->branches[i].nested++;
				default: break;
			}
		}
	}
}

void r_meta_rip_resolve(r_disassembler * disassembler, r_file * file)
{
	char buf[256];
	memset(buf, 0, 256);
	//Add comments for RIP relative addresses (x64 only)
	for (int j = 0; j < disassembler->num_instructions; j++) {
		r_disasm * disas = disassembler->instructions[j];
		for (int k = 0; k < disas->num_operands; k++) {
			int n = r_meta_rip_relative(disas->op[k]);
			if (n != 0) {
				memset(buf, 0, 20);
				snprintf(buf, 20, "%#x", n + disas->address+disas->used_bytes);
				disas->metadata->comment = strdup(buf);
				break;
			}
		} 
	}
}

void r_meta_reloc_resolve(r_disassembler * disassembler, r_file * file)
{

	//Find reloc symbols are correct the addresses
	for (int i = 0; i < file->num_symbols; i++) {
		rsymbol sym = file->symbols[i];
		if (!R_RELOC(sym.type)) continue;
		for (int j = 0; j < disassembler->num_instructions; j++) {
			r_disasm * disas = disassembler->instructions[j];
			int len = 0;
			if (r_meta_find_addr(disas->metadata, sym.addr64, META_ADDR_DATA)) {
				sym.addr64 = disas->address;
				sym.type = R_FUNC;
				file->symbols[i] = sym;
			} else if (r_meta_isaddr(disas->metadata->comment, &len)) {
				//Check if the comment is an address (for rip relative relocs)
				int base = 16;
				if (strlen(disas->metadata->comment) > 2 && (disas->metadata->comment[1] == 'x' || disas->metadata->comment[1] == 'X')) base = 0;
				uint64_t num = (uint64_t)strtol(disas->metadata->comment, NULL, base);
				if (num == sym.addr64) {
					sym.addr64 = disas->address;
					sym.type = R_FUNC;
					file->symbols[i] = sym;
				}
			}
		}				
	}
}

void r_meta_symbol_replace(r_disassembler * disassembler, r_file * file)
{
	char buf[256];
	memset(buf, 0, 256);
	//Add labels for code that is the address of a file symbol
	//And Find references to symbols and replace address with symbol name
	for (int j = 0; j < disassembler->num_instructions; j++) {
		r_disasm * disas = disassembler->instructions[j];
		for (int i = 0; i < file->num_symbols; i++) {
			rsymbol  sym = file->symbols[i];
			if (!sym.name) continue;
			if (disas->metadata->type == r_tcall && sym.type == R_FUNC && r_meta_find_addr(disas->metadata, sym.addr64, META_ADDR_BRANCH) && !disas->metadata->comment) {
				if (disas->op[0]) {
					disas->metadata->comment = disas->op[0];
					disas->op[0] = NULL;
					//free(disas->op[0]);
				}
				disas->op[0] = strdup(sym.name);
			}
			if (sym.type == R_FUNC && disas->address == sym.addr64) {
				snprintf(buf, 256, "sym.%s", sym.name);
				disas->metadata->label = strdup(buf);
			}
		}
	}
}

void r_meta_string_replace(r_disassembler * disassembler, r_file * file)
{
	char buf[256];
	memset(buf, 0, 256);
	for (int j = 0; j < disassembler->num_instructions; j++) {
		r_disasm * disas = disassembler->instructions[j];
		//Find references to strings and insert them
		for (int i = 0; i < file->num_strings; i++) {
			if ((disas->metadata->type == r_tdata) && file->strings[i].addr64 != 0 && r_meta_find_addr(disas->metadata, file->strings[i].addr64, META_ADDR_DATA)) {
				for (int k = 0; k < disas->num_operands; k++) {
					int len = 0;
					if (r_meta_isaddr(disas->op[k], &len)){
						int base = 16;
						if (strlen(disas->op[k]) > 2 && (disas->op[k][1] == 'x' || disas->op[k][1] == 'X')) base = 0;
						uint64_t num = (uint64_t)strtol(disas->op[k], NULL, base);
						if (num == file->strings[i].addr64) {
							disas->metadata->comment = disas->op[k];
							disas->op[k] = NULL;
							//free(disas->op[k]);
							snprintf(buf, 256, " \"%s\"", file->strings[i].string);
							disas->op[k] = strdup(buf);
						}
					}
					
				}
			}
		}
	}
}

uint64_t r_meta_get_address(char * operand, int * status)
{
	//Status of 0 means no address found. 1 means address found. 2 Means indirect address found (eg 0x400 vs [0x400])
	*status = 0;
	int len = 0;
	if (r_meta_isaddr(operand, &len)) {
		*status = 1;
		int base = 16;
		if (strlen(operand) > 2 && (operand[1] == 'x' || operand[1] == 'X')) base = 0;
		return (uint64_t)strtol(operand, NULL, base);
	} else {
		*status = 2;
		int size = strlen(operand);
		for (int i = 0; i < size; i++) {
			int len = 0;
			r_meta_isaddr(operand+i,&len);
			//Found the indirect address
			if (operand[i+len] == ']') {
				char * op2 = strdup(operand+i);
				op2[len] = 0;
				int base = 16;
				if (strlen(operand) > 2 && (operand[1] == 'x' || operand[1] == 'X')) base = 0;
				uint64_t n = strtol(op2, NULL, base);
				free(op2);
				return n;
			}
		}
	}	
	return 0;
}

//Returns 1 if the string is an address, 0 if it is not. Sets length to the number of characters that are a valid string
int r_meta_isaddr(char * operand, int * len)
{
	if (!operand) return 0;

	int s = strlen(operand);
	for (int i = 0; i < s; i++) {
		*len = i;
		//Empty if statements just for the sake of not having a very long line
		if (operand[i] >= 0x30 && operand[i] < 0x40) continue;
		else if(operand[i] >= 'a'  && operand[i] <= 'f') continue;
		else if (operand[i] >= 'A' && operand[i] <= 'F') continue;
		else if (i == 1 && operand[0] == '0' && (operand[i] == 'x' || operand[i] == 'X')) continue;
		else return 0;
	}

	return 1;
}

int r_meta_rip_relative(char * operand)
{
	if (!operand) return 0;

	int s = strlen(operand);
	for (int i = 0; i < s; i++) {
		if (!strncmp(operand + i, "rip+", 4)) {
			char * opaddr = strdup(operand+i+4);
			//rip operands are usually like [rip+0xaddr] so remove ']' at the end
			int len = 0;
			r_meta_isaddr(opaddr, &len);
			//It is a valid address up until the ']' indirect part
			if (opaddr[len]==']') {
				opaddr[len] = 0;
				int base = 16;
				if (strlen(opaddr) > 2 && (opaddr[1] == 'x' || opaddr[1] == 'X')) base = 0;
				
				uint64_t num =  (uint64_t)strtol(opaddr, NULL, base);
				free(opaddr);
				return num;
			}
			free(opaddr);
		}
	}

	return 0;
}


void r_add_xref(r_disasm * to, r_disasm * from)
{
	from->metadata->num_xreffrom++;
	if (from->metadata->num_xreffrom == 1) {
		from->metadata->xref_from = malloc(sizeof(uint64_t));
	} else {
		from->metadata->xref_from = realloc(from->metadata->xref_from, sizeof(uint64_t) * from->metadata->num_xreffrom);
	}

	from->metadata->xref_from[from->metadata->num_xreffrom-1] = to->address;

	to->metadata->num_xrefto++;
	if (to->metadata->num_xrefto == 1) {
		to->metadata->xref_to = malloc(sizeof(uint64_t));
	} else {
		to->metadata->xref_to = realloc(to->metadata->xref_to, sizeof(uint64_t) * to->metadata->num_xrefto);
	}
	to->metadata->xref_to[to->metadata->num_xrefto-1] = from->address;
}

void r_meta_find_xrefs(r_disassembler * disassembler, r_file * file)
{
	for (int i = 0; i < disassembler->num_instructions; i++) {
		r_disasm * instr1 = disassembler->instructions[i];

		for (int j = 0; j < disassembler->num_instructions; j++) {
			if (j==i) continue;
			r_disasm * instr2 = disassembler->instructions[j];

			for (int k = 0; k < instr2->metadata->num_addr; k++) {
				if (instr2->metadata->addresses[k] == instr1->address) {
					r_add_xref(instr1, instr2);
				}
			}
		}
	}	
}

r_analyzer * r_analyzer_init()
{
	r_analyzer * anal = malloc(sizeof(r_analyzer));

	anal->num_functions = 0;
	anal->functions = NULL;
	anal->num_branches = 0;
	anal->branches = NULL;
	anal->function = 1;

	return anal;
}


void r_analyzer_destroy(r_analyzer * anal)
{
	if (!anal) return;
	
	for (int i = 0; i < anal->num_functions; i++) {
		free(anal->functions[i].name);
	}
	for (int i = 0; i < anal->num_branches; i++) {

	}

	free(anal->functions);
	free(anal->branches);
	free(anal);
}