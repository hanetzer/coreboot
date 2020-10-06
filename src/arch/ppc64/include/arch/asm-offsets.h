#ifndef ASM_OFFSETS_H
#define ASM_OFFSETS_H
/* Derived from asm/asm-offsets.s by make_offsets.sh */

#define CPUTHREAD_PIR 8 /* offsetof(struct cpu_thread, pir) */
#define CPUTHREAD_SAVE_R1 64 /* offsetof(struct cpu_thread, save_r1) */
#define CPUTHREAD_STATE 40 /* offsetof(struct cpu_thread, state) */
#define CPUTHREAD_IN_OPAL_CALL 80 /* offsetof(struct cpu_thread, in_opal_call) */
#define CPUTHREAD_QUIESCE_OPAL_CALL 84 /* offsetof(struct cpu_thread, quiesce_opal_call) */
#define CPUTHREAD_CUR_TOKEN 144 /* offsetof(struct cpu_thread, current_token) */
#define CPUTHREAD_GAP 936 /* sizeof(struct cpu_thread) + STACK_SAFETY_GAP */
#define STACK_TYPE 128 /* offsetof(struct stack_frame, type) */
#define STACK_MAGIC 120 /* offsetof(struct stack_frame, magic) */
#define STACK_LOCALS 112 /* offsetof(struct stack_frame, locals) */
#define STACK_GPR0 136 /* offsetof(struct stack_frame, gpr[0]) */
#define STACK_GPR1 144 /* offsetof(struct stack_frame, gpr[1]) */
#define STACK_GPR2 152 /* offsetof(struct stack_frame, gpr[2]) */
#define STACK_GPR3 160 /* offsetof(struct stack_frame, gpr[3]) */
#define STACK_GPR4 168 /* offsetof(struct stack_frame, gpr[4]) */
#define STACK_GPR5 176 /* offsetof(struct stack_frame, gpr[5]) */
#define STACK_GPR6 184 /* offsetof(struct stack_frame, gpr[6]) */
#define STACK_GPR7 192 /* offsetof(struct stack_frame, gpr[7]) */
#define STACK_GPR8 200 /* offsetof(struct stack_frame, gpr[8]) */
#define STACK_GPR9 208 /* offsetof(struct stack_frame, gpr[9]) */
#define STACK_GPR10 216 /* offsetof(struct stack_frame, gpr[10]) */
#define STACK_GPR11 224 /* offsetof(struct stack_frame, gpr[11]) */
#define STACK_GPR12 232 /* offsetof(struct stack_frame, gpr[12]) */
#define STACK_GPR13 240 /* offsetof(struct stack_frame, gpr[13]) */
#define STACK_GPR14 248 /* offsetof(struct stack_frame, gpr[14]) */
#define STACK_GPR15 256 /* offsetof(struct stack_frame, gpr[15]) */
#define STACK_GPR16 264 /* offsetof(struct stack_frame, gpr[16]) */
#define STACK_GPR17 272 /* offsetof(struct stack_frame, gpr[17]) */
#define STACK_GPR18 280 /* offsetof(struct stack_frame, gpr[18]) */
#define STACK_GPR19 288 /* offsetof(struct stack_frame, gpr[19]) */
#define STACK_GPR20 296 /* offsetof(struct stack_frame, gpr[20]) */
#define STACK_GPR21 304 /* offsetof(struct stack_frame, gpr[21]) */
#define STACK_GPR22 312 /* offsetof(struct stack_frame, gpr[22]) */
#define STACK_GPR23 320 /* offsetof(struct stack_frame, gpr[23]) */
#define STACK_GPR24 328 /* offsetof(struct stack_frame, gpr[24]) */
#define STACK_GPR25 336 /* offsetof(struct stack_frame, gpr[25]) */
#define STACK_GPR26 344 /* offsetof(struct stack_frame, gpr[26]) */
#define STACK_GPR27 352 /* offsetof(struct stack_frame, gpr[27]) */
#define STACK_GPR28 360 /* offsetof(struct stack_frame, gpr[28]) */
#define STACK_GPR29 368 /* offsetof(struct stack_frame, gpr[29]) */
#define STACK_GPR30 376 /* offsetof(struct stack_frame, gpr[30]) */
#define STACK_GPR31 384 /* offsetof(struct stack_frame, gpr[31]) */
#define STACK_CR 392 /* offsetof(struct stack_frame, cr) */
#define STACK_XER 396 /* offsetof(struct stack_frame, xer) */
#define STACK_DSISR 400 /* offsetof(struct stack_frame, dsisr) */
#define STACK_CTR 408 /* offsetof(struct stack_frame, ctr) */
#define STACK_LR 416 /* offsetof(struct stack_frame, lr) */
#define STACK_PC 424 /* offsetof(struct stack_frame, pc) */
#define STACK_MSR 432 /* offsetof(struct stack_frame, msr) */
#define STACK_CFAR 440 /* offsetof(struct stack_frame, cfar) */
#define STACK_SRR0 448 /* offsetof(struct stack_frame, srr0) */
#define STACK_SRR1 456 /* offsetof(struct stack_frame, srr1) */
#define STACK_HSRR0 464 /* offsetof(struct stack_frame, hsrr0) */
#define STACK_HSRR1 472 /* offsetof(struct stack_frame, hsrr1) */
#define STACK_DAR 480 /* offsetof(struct stack_frame, dar) */
#define STACK_FRAMESIZE 496 /* sizeof(struct stack_frame) */
#define INT_FRAMESIZE 784 /* (sizeof(struct stack_frame) + REDZONE_SIZE) */
#endif /* ASM_OFFSETS_H */
