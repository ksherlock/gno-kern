/*	$Id: gno.h,v 1.1 1998/02/02 08:18:29 taubert Exp $ */

/*
         gno.h

         libgno interface file for GNO Kernel
         v1.0b2
         Copyright 1991-1998, Procyon Inc.
*/

#include <types.h>

#ifndef udispatch
#define udispatch  0xE10008
#endif
extern pascal int   kernVersion(void) inline(0x0403, udispatch);
extern pascal int   kernStatus(void) inline(0x0603, udispatch);


extern int kern_printf(const char *, ...);
extern void kern_print(const char *);

int KERNexecve(int *ERRNO, char *cmdline, char *filename);
int Kscreate(int *ERRNO, int count);
int KERNssignal(int *ERRNO, int sem);
int Kscount(int *ERRNO, int sem);
int KERNsdelete(int *ERRNO, int sem);
int KERNkill(int *ERRNO, int signum, int pid);
void *Ksignal(int *ERRNO, void (*func)(int, int), int sig );
longword Ksigblock(int *ERRNO, longword mask);
longword Ksigsetmask(int *ERRNO, longword mask);

/*
 * These functions use the pascal/toolcall protocol,
 * ie, return value on stack.  wrap with pha/pla
 * so they can be used in C.
 */

#define Kexecve(__e, __p1, __p2)	\
	{ asm { pha } KERNexecve(__e, __p1, __p2); asm { pla } }
#define Kssignal(__e, __p1)		\
	{ asm { pha } KERNssignal(__e, __p1); asm { pla } }
#define Ksdelete(__e, __p1)		\
	{ asm { pha } KERNsdelete(__e, __p1); asm { pla } }
#define Kkill(__e, __p1, __p2)		\
	{ asm { pha } KERNkill(__e, __p1, __p2); asm { pla } }
#define Kkvmsetproc(__e, __p1)		\
	{ asm { pha } KERNkvmsetproc(__e, __p1); asm { pla } }

extern int errno;

extern void PANIC(const char *);

extern void InitRefnum(void);
extern void AddRefnum(int, int);
