#include "x86256.h"

void xmov(ul regd, ul regs) {
	_xmov(regd, regs);
}

void xadd(ul regd, ul regs) {
	_xadd(regd, regs);
}

void xadc(ul regd, ul regs) {
	_xadc(regd, regs);
}

void xinc(ul regd) {
	_xinc(regd);
}

void xsub(ul regd, ul regs) {
	_xsub(regd, regs);
}

void xsbb(ul regd, ul regs) {
	_xsbb(regd, regs);
}

void xdec(ul regd) {
	_xdec(regd);
}

void xmul(ul regd_lo, ul regd_hi, ul regs1, ul regs2) {
	_xmul(regd_lo, regd_hi, regs1, regs2);
}

void xshr(ul regs, ul sh) {
	int i, q;

	if(sh[0] > W * D) {						// sh > 256 => regs = 0
		for(i = 0; i < D; i++) {
			regs[i] = 0UL;
		}
	} else {
		q = sh[0] / W;
		for(i = 0; i < D - q ; i++) {
			regs[i] = regs[i + q];
		}
		for(i = D - q; i < D; i++) {
			regs[i] = 0UL;
		}
		sh[0] = sh[0] % W;
		if(sh[0] != 0UL) {
			_xshr(regs, sh);
		}
	}
}

void xshl(ul regs, ul sh) {
	int i, q;

	if(sh[0] > W * D) {						// sh > 256 => regs = 0
		for(i = 0; i < D; i++) {
			regs[i] = 0UL;
		}
	} else {
		q = sh[0] / W;
		for(i = D - 1; i >= q ; i--) {
			regs[i] = regs[i - q];
		}
		for(i = 0; i < q; i++) {
			regs[i] = 0UL;
		}
		sh[0] = sh[0] % W;
		_xshl(regs, sh);
	}
}

//ul_t xlbl() {
//	ul_t addr = _xlbl();
//	return addr;
//}
//
//void xje(ul_t lbl, ul regs1, ul regs2) {
//	int i, flag = 1;
//
//	for(i = 0; i < D; i++) {
//		if(regs1[i] != regs2[i]) {
//			flag = -1;
//		}
//	}
//	if(flag == 1) {
//		_xjmp(lbl);
//	}
//}
