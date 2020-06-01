#ifndef X86256_H_
#define X86256_H_

typedef unsigned long ul_t;
typedef unsigned long *ul;

#define W 64						// Word size of the architecture
#define D 4							// Max number of digits (4 for 256 bit virtual architecture)

ul_t reg1[D];
ul_t reg2[D];
ul_t reg3[D];
ul_t reg4[D];
ul_t reg5[D];
ul_t reg6[D];
ul_t reg7[D];

// 256 bit equivalent of "mov regd, regs"
void _xmov(ul regd, ul regs);
void xmov(ul regd, ul regs);

// 256 bit equivalent of "add regd, regs"
void _xadd(ul regd, ul regs);
void xadd(ul regd, ul regs);

// 256 bit equivalent of "adc regd, regs"
void _xadc(ul regd, ul regs);
void xadc(ul regd, ul regs);

// 256 bit equivalent of "inc regd"
void _xinc(ul regd);
void xinc(ul regd);

// 256 bit equivalent of "xsub regd, regs"
void _xsub(ul regd, ul regs);
void xsub(ul regd, ul regs);

// 256 bit equivalent of "sbb regd, regs"
void _xsbb(ul regd, ul regs);
void xsbb(ul regd, ul regs);

// 256 bit equivalent of "dec regd"
void _xdec(ul regd);
void xdec(ul regd);

// 256 bit equivalent of "mul regs1, regs2"
// hi part is written to regd_hi
// lo part is written to regd_lo
void _xmul(ul regd_lo, ul regd_hi, ul regs1, ul regs2);
void xmul(ul regd_lo, ul regd_hi, ul regs1, ul regs2);

// 256 bit equivalent of "shr regs1, sh"
// sh is a 256 bit immediate value or a 256 bit virtual register
void _xshr(ul regs, ul sh);
void xshr(ul regs, ul sh);

// 256 bit equivalent of "shl regs1, sh"
// sh is a 256 bit immediate value or a 256 bit virtual register
void _xshl(ul regs, ul sh);
void xshl(ul regs, ul sh);

// 256 bit equivalent of "push regs"
void _xpush(ul regs);

// 256 bit equivalent of "pop regs"
void _xpop(ul regs);

//// Equivalent of putting a label
//// Returns the label as an address
//ul_t _xlbl();
//ul_t xlbl();
//
//// Jumps to a previously defined label (by _xlbl)
//// Cannot jump a later defined label
//void _xjmp(ul_t lbl);
//
//// Jumps to a previously defined label (by _xlbl)
//// if regs1 == regs2
//// Cannot jump a later defined label
//void xje(ul_t lbl, ul regs1, ul regs2);


#endif
