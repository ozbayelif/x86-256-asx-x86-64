#ifndef TEST_H_
#define TEST_H_

#include "x86256.h"

void print_256(ul z, const char *s);

void print_512(ul z_lo, ul z_hi, const char *s);

void rand_256(ul z);

char _xcf();

char _xbf();

void xmov_test(int THRESHOLD);

void xadd_magma_test(int THRESHOLD);

void xadc_magma_test(int THRESHOLD);

void xinc_magma_test(int THRESHOLD);

void xsub_magma_test(int THRESHOLD);

void xsbb_magma_test(int THRESHOLD);

void xdec_magma_test(int THRESHOLD);

void xmul_magma_test(int THRESHOLD);

void xshr_magma_test(int THRESHOLD);

void xshl_magma_test(int THRESHOLD);

void xpush_pop_test(int THRESHOLD);

#endif
