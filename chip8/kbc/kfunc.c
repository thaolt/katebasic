uint8_t kfuncUseMultiply = 0;

/**
 * v0: argument 1 
 * v1: argument 2
 * return value: v0 
 */
void kfuncMultiply() {
	if (!kfuncUseMultiply) {
		_asm(okfunc, "\t.multiply:");
		_asm(okfunc, "\t\tmov\tv3,\tv1");
		_asm(okfunc, "\t\tse\tv3,\t0");
		_asm(okfunc, "\t\tadd\tv0,\tv1");
		_asm(okfunc, "\t\tjmp\t$+2");
		_asm(okfunc, "\t\tjmp\t.multiply");
		_asm(okfunc, "\tret");
	}
	_asm(cscope, "\tcall\tkfunc.multiply");
}