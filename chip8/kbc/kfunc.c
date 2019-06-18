uint8_t kfuncUseMultiply = 0;

/**
 * v0: argument 1 
 * v1: argument 2
 * return value: v0 
 */
void kfuncMultiply() {
	if (!kfuncUseMultiply) {
		_asm(okfunc, "\t.multiply:");
		_asm(okfunc, "\t\tmov\tv3,\t1");
		_asm(okfunc, "\t\tmov\tv2,\tv1");
		_asm(okfunc, "\t\tmov\tv1,\tv0");
		_asm(okfunc, "\t\tmov\tv0,\t0");
		_asm(okfunc, "\t\tsne\tv2,\t0");
		_asm(okfunc, "\t\tjmp\t$ + 8");
		_asm(okfunc, "\t\tadd\tv0,\tv1");
		_asm(okfunc, "\t\tsub\tv2,\tv3");
		_asm(okfunc, "\t\tjmp\t$ - 8");
		_asm(okfunc, "\tret");
		kfuncUseMultiply = 1;
	}
	_asm(cscope, "\tcall\tkfunc.multiply");
}
