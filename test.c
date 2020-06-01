#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "x86256.h"

void print_256(ul z, const char *s) {
	int i;
	printf("%s := %lu", s, z[0]);
	for(i = 1; i < D; i++) {
		printf(" + %lu * (2^%d)^%d", z[i], W, i);
	}
	printf(";\n");
}

void print_512(ul z_lo, ul z_hi, const char *s) {
	int i;
	printf("%s := %lu", s, z_lo[0]);
	for(i = 1; i < D; i++) {
		printf(" + %lu * (2^%d)^%d", z_lo[i], W, i);
	}
	for(i = 0; i < D; i++) {
			printf(" + %lu * (2^%d)^%d", z_hi[i], W, i + D);
		}
	printf(";\n");
}

void rand_256(ul z) {
	int i;
	for(i = 0; i < D; i++) {
		z[i] = (ul_t)rand() * (ul_t)rand() * (ul_t)rand() * (ul_t)rand();
	}
}

void xmov_test(int THRESHOLD) {
	ul_t a[D], b[D];
	int i, j, success = 0, fail = 0, flag = 1;

	printf("/*Testing xmov...*/\n");

	for(i = 0; i < THRESHOLD; i++) {
		flag = 1;
		rand_256(a);
		xmov(reg1, a);							// Immediate to virtual register
		for(j = 0; j < D; j++) {
			if(a[j] != reg1[j]) {
				flag = -1;
			}
		}
		rand_256(b);
		xmov(reg2, b);
		xmov(reg1, reg2);						// Virtual register to virtual register
		for(j = 0; j < D; j++) {
			if(reg1[j] != reg2[j]) {
				flag = -1;
			}
		}
		if(flag == 1) {
			success++;
		} else {
			fail++;
		}
	}
	printf("/*Test finished*/\n");

	printf("Success: %d\n", success);
	printf("Fail: %d\n", fail);
}

void xadd_magma_test(int THRESHOLD) {
	ul_t a[D], b[D];
	int i;
	char cf;

	printf("/*Testing xadd...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		rand_256(b);
		xmov(reg1, a);
		xmov(reg2, b);

		print_256(reg1, "reg1");
		print_256(reg2, "reg2");

		xadd(reg1, reg2);
		cf = _xcf();

		print_256(reg1, "res");
		printf("cf := %d;\n", (int)cf);

		printf("if (res eq reg1 + reg2) then\n");
		printf("    success := success + 1;\n");
		printf("elif (cf eq 1) then\n");
		printf("    if (res eq (reg1 + reg2) mod 2^256) then\n");
		printf("        success := success + 1;\n");
		printf("    else\n");
		printf("        fail := fail + 1;\n");
		printf("    end if;\n");
		printf("else\n");
		printf("    fail := fail + 1;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xadc_magma_test(int THRESHOLD) {
	ul_t a[D], b[D], c[D], d[D];
	int i;
	char cf1, cf2;

	printf("/*Testing xadc...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		rand_256(b);
		rand_256(c);
		rand_256(d);
		xmov(reg1, a);
		xmov(reg2, b);
		xmov(reg3, c);
		xmov(reg4, d);

		print_256(reg1, "reg1");
		print_256(reg2, "reg2");
		print_256(reg3, "reg3");
		print_256(reg4, "reg4");

		xadd(reg1, reg2);
		cf1 = _xcf();
		print_256(reg1, "res1");
		printf("cf1 := %d;\n", (int)cf1);

		xadc(reg3, reg4);
		cf2 = _xcf();
		printf("cf2 := %d;\n", (int)cf2);
		print_256(reg3, "res2");

		printf("if (cf1 eq 1) then\n");
		printf("    if (res2 eq reg3 + reg4 + 1) then\n");
		printf("        success := success + 1;\n");
		printf("    elif (cf2 eq 1) then\n");
		printf("        if (res2 eq (reg3 + reg4 + 1) mod 2^256) then\n");
		printf("			success := success + 1;\n");
		printf("		else\n");
		printf("			fail := fail + 1;\n");
		printf("		end if;\n");
		printf("    else\n");
		printf("        fail := fail + 1;\n");
		printf("	end if;\n");
		printf("else\n");
		printf("    if (res2 eq reg3 + reg4) then\n");
		printf("        success := success + 1;\n");
		printf("    elif (cf2 eq 1) then\n");
		printf("        if (res2 eq (reg3 + reg4) mod 2^256) then\n");
		printf("			success := success + 1;\n");
		printf("		else\n");
		printf("			fail := fail + 1;\n");
		printf("		end if;\n");
		printf("    else\n");
		printf("        fail := fail + 1;\n");
		printf("	end if;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xinc_magma_test(int THRESHOLD) {
	ul_t a[D];
	int i;
	char cf;

	printf("/*Testing xinc...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		xmov(reg1, a);

		print_256(reg1, "reg1");

		xinc(reg1);
		cf = _xcf();
		print_256(reg1, "res");
		printf("cf := %d;\n", (int)cf);

		printf("if (cf eq 1) then\n");
		printf("    if (res eq (reg1 + 1) mod 2^256) then\n");
		printf("        success := success + 1;\n");
		printf("    else\n");
		printf("		fail := fail + 1;\n");
		printf("	end if;\n");
		printf("else\n");
		printf("	if (res eq reg1 + 1) then\n");
		printf("        success := success + 1;\n");
		printf("    else\n");
		printf("		fail := fail + 1;\n");
		printf("	end if;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xsub_magma_test(int THRESHOLD) {
	ul_t a[D], b[D];
	int i;
	char bf;

	printf("/*Testing xsub...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		rand_256(b);
		xmov(reg1, a);
		xmov(reg2, b);

		print_256(reg1, "reg1");
		print_256(reg2, "reg2");

		xsub(reg1, reg2);
		bf = _xbf();

		print_256(reg1, "res");
		printf("bf := %d;\n", (int)bf);

		printf("if (res eq reg1 - reg2) then\n");
		printf("    success := success + 1;\n");
		printf("elif (bf eq 1) then\n");
		printf("    if (res eq (reg1 - reg2 + 2^256)) then\n");
		printf("        success := success + 1;\n");
		printf("    else\n");
		printf("        fail := fail + 1;\n");
		printf("    end if;\n");
		printf("else\n");
		printf("    fail := fail + 1;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xsbb_magma_test(int THRESHOLD) {
	ul_t a[D], b[D], c[D], d[D];
	int i;
	char bf1, bf2;

	printf("/*Testing xsbb...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		rand_256(b);
		rand_256(c);
		rand_256(d);
		xmov(reg1, a);
		xmov(reg2, b);
		xmov(reg3, c);
		xmov(reg4, d);

		print_256(reg1, "reg1");
		print_256(reg2, "reg2");
		print_256(reg3, "reg3");
		print_256(reg4, "reg4");

		xsub(reg1, reg2);
		bf1 = _xbf();
		print_256(reg1, "res1");
		printf("bf1 := %d;\n", (int)bf1);

		xsbb(reg3, reg4);
		bf2 = _xbf();
		printf("bf2 := %d;\n", (int)bf2);
		print_256(reg3, "res2");

		printf("if (bf1 eq 1) then\n");
		printf("    if (res2 eq reg3 - reg4 - 1) then\n");
		printf("        success := success + 1;\n");
		printf("    elif (bf2 eq 1) then\n");
		printf("        if (res2 eq (reg3 - reg4 - 1 + 2^256)) then\n");
		printf("			success := success + 1;\n");
		printf("		else\n");
		printf("			fail := fail + 1;\n");
		printf("		end if;\n");
		printf("    else\n");
		printf("        fail := fail + 1;\n");
		printf("	end if;\n");
		printf("else\n");
		printf("    if (res2 eq reg3 - reg4) then\n");
		printf("        success := success + 1;\n");
		printf("    elif (bf2 eq 1) then\n");
		printf("        if (res2 eq (reg3 - reg4 + 2^256)) then\n");
		printf("			success := success + 1;\n");
		printf("		else\n");
		printf("			fail := fail + 1;\n");
		printf("		end if;\n");
		printf("    else\n");
		printf("        fail := fail + 1;\n");
		printf("	end if;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xdec_magma_test(int THRESHOLD) {
	ul_t a[D];
	int i;
	char bf;

	printf("/*Testing xdec...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		xmov(reg1, a);

		print_256(reg1, "reg1");

		xdec(reg1);
		bf = _xbf();
		print_256(reg1, "res");
		printf("bf := %d;\n", (int)bf);

		printf("if (bf eq 1) then\n");
		printf("    if (res eq (reg1 - 1 + 2^256)) then\n");
		printf("        success := success + 1;\n");
		printf("    else\n");
		printf("		fail := fail + 1;\n");
		printf("	end if;\n");
		printf("else\n");
		printf("	if (res eq reg1 - 1) then\n");
		printf("        success := success + 1;\n");
		printf("    else\n");
		printf("		fail := fail + 1;\n");
		printf("	end if;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xmul_magma_test(int THRESHOLD) {
	ul_t a[D], b[D], c[D], d[D];
	int i;

	printf("/*Testing xmul...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		rand_256(b);
		xmov(reg1, a);
		xmov(reg2, b);
		xmov(reg3, c);
		xmov(reg4, d);

		// 256 * 2 bit bas

		print_256(reg1, "reg1");
		print_256(reg2, "reg2");

		xmul(reg3, reg4, reg1, reg2);

		print_512(reg3, reg4, "res");

		printf("if (res eq reg1 * reg2) then\n");
		printf("	success := success + 1;\n");
		printf("else\n");
		printf("	fail := fail + 1;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xshr_magma_test(int THRESHOLD) {
	ul_t a[D], sh[D];
	int i, j;

	printf("/*Testing xshr...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		xmov(reg1, a);
		sh[0] = rand() % (W * D);
		for(j = 1; j < D; j++) {
			sh[j] = 0UL;
		}

		print_256(reg1, "reg1");
		print_256(sh, "sh");

		xshr(reg1, sh);

		print_256(reg1, "res");

		printf("if (res eq reg1 div 2^sh) then\n");
		printf("	success := success + 1;\n");
		printf("else\n");
		printf("	fail := fail + 1;\n");
		printf("	assert 1 eq 2;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xshl_magma_test(int THRESHOLD) {
	ul_t a[D], sh[D];
	int i, j;

	printf("/*Testing xshr...*/\n");

	printf("clear;\n");
	printf("success := 0;\n");
	printf("fail := 0;\n\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		xmov(reg1, a);
		sh[0] = rand() % (W * D);
		for(j = 1; j < D; j++) {
			sh[j] = 0UL;
		}

		print_256(reg1, "reg1");
		print_256(sh, "sh");

		xshl(reg1, sh);

		print_256(reg1, "res");

		printf("if (res eq (reg1 * 2^sh mod 2^256)) then\n");
		printf("	success := success + 1;\n");
		printf("else\n");
		printf("	fail := fail + 1;\n");
		printf("end if;\n");
	}
	printf("/*Test finished*/\n");

	printf("\nsuccess;\n");
	printf("fail;\n");
}

void xpush_pop_test(int THRESHOLD) {
	ul_t a[D];
	int i, j, success = 0, fail = 0, flag = 1;

	printf("/*Testing _xpush and _xpop...*/\n");

	for(i = 0; i < THRESHOLD; i++) {
		rand_256(a);
		xmov(reg1, a);

		print_256(reg1, "reg1");

		_xpush(reg1);

		_xpop(reg2);

		print_256(reg2, "reg2");

		for(j = 0; j < D; j++) {
			if(reg1[j] != reg2[j]) {
				flag = -1;
			}
		}
		if(flag != 1) {
			success++;
		} else {
			fail++;
		}
	}
	printf("/*Test finished*/\n");

	printf("Success: %d\n", success);
	printf("Fail: %d\n", fail);
}

//void xjmp_test() {
//	int i = 0;
//
//	printf("Start\n");
//	ul_t lbl = _xlbl();
//	i++;
//	printf("i = %d\n", i);
//	if(i == 5) {
//		printf("End\n");
//	} else {
//		printf("I jump back to the label\n");
//		_xjmp(lbl);
//	}
//}

