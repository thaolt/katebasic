uint8_t kfuncUseMultiply = 0;
uint8_t kfuncUseDivide = 0;
uint8_t kfuncPutHex = 0;

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
	_asm(cscope->oasm, "\tcall\tkfunc.multiply");
}

/**
 * v0: argument 1 
 * v1: argument 2
 * return value: v0 
 * modulo: vF
 */
void kfuncDivide() {
	if (!kfuncUseDivide) {
		_asm(okfunc, "\t.divide:");
		_asm(okfunc, "\t\tmov\tv3,\t1");
		_asm(okfunc, "\t\tmov\tv2,\tv1");
		_asm(okfunc, "\t\tmov\tv1,\tv0");
		_asm(okfunc, "\t\tsne\tv2,\t1");
		_asm(okfunc, "\t\tjmp\t$ + 8");
		_asm(okfunc, "\t\tsub\tv0,\tv1");
		_asm(okfunc, "\t\tsub\tv2,\tv3");
		_asm(okfunc, "\t\tjmp\t$ - 8");
		_asm(okfunc, "\tret");
		kfuncUseDivide = 1;
	}
	_asm(cscope->oasm, "\tcall\tkfunc.divide");
}

/**
 * v0: number(hex)
 * v1: x
 * v2: y
 * collided: vF
 * height always 5
 */
void puthex() {
	if (!kfuncPutHex) {
		_asm(okfunc, "\t.puthex:");
		_asm(okfunc, "\thex\tv0");
		_asm(okfunc, "\tdraw\tv1,\tv2,\t5");
		_asm(okfunc, "\tret");
		kfuncPutHex = 1;
	}
	_asm(cscope->oasm, "\tcall\tkfunc.puthex");
}

