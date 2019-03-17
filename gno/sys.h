/*	$Id: sys.h,v 1.1 1998/02/02 08:19:10 taubert Exp $ */

#ifndef KERN_SYS_H
#define KERN_SYS_H

#include "proc.h"

#define Kgetpid() kp->truepid

typedef struct pgrp {
    word pgrpref; /* number of references to this pgrp - tty and process */
} pgrp;

fdentryPtr allocFD(int *fdn);
void copygsstr(void *, void *);
void nfree(void *);

#define PROC procPtr
extern struct pentry *procPtr;

#endif
