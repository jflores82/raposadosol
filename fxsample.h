#ifndef FX_SAMPLE_H
#define FX_SAMPLE_H
#define LDIXHA .dw #0x67DD
#define LDIXLA .dw #0x6FDD
#define LDAIXH .dw #0x7CDD
#define LDAIXL .dw #0x7DDD
#define ORIXL .dw #0xB5DD
#define LDIYHA .dw #0x67FD
#define LDIYLA .dw #0x6FFD
#define LDAIYH .dw #0x7CFD
#define LDAIYL .dw #0x7DFD
#define ORIYL .dw #0xB5FD
#define LDDIYH .dw #0x54FD
#define LDDIYL .dw #0x55FD
#define LDEIYH .dw #0x5CFD
#define LDEIYL .dw #0x5DFD

void initPSG(void *psginit) __z88dk_fastcall;
void PlaySample (void *sample) __z88dk_fastcall;

#endif //FX_SAMPLE_H
