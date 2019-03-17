
#ifndef KERN_SIGNAL_H
#define KERN_SIGNAL_H

#define NSIG 32

struct sigrec {
   unsigned long signalmask;
   unsigned long sigpending;	/* describes which signals are pending */
   void (*v_signal[NSIG])(int, int);
   unsigned long u_sigmask[NSIG];
   unsigned long u_oldmask;	/* for sigpause & sigblock */
   int sigindex;		/* which signal to check next for processing */
};

union wait {
    int w_status;       /* used in syscall */
    /*
     * Terminated process status.
     */
    struct {
        unsigned int    w_Termsig:7,    /* termination signal */
                w_Coredump:1,   /* core dump indicator */
                w_Retcode:8;    /* exit code if w_termsig==0 */
    } w_T;
    /*
     * Stopped process status.  Returned
     * only for traced children unless requested
     * with the WUNTRACED option bit.
     */
    struct {
        unsigned int    w_Stopval:8,    /* == W_STOPPED if stopped */
                w_Stopsig:8;    /* signal that stopped us */
    } w_S;
};
#define w_termsig   w_T.w_Termsig
#define w_coredump  w_T.w_Coredump
#define w_retcode   w_T.w_Retcode
#define w_stopval   w_S.w_Stopval
#define w_stopsig   w_S.w_Stopsig


#define WNOHANG   1 /* dont hang in wait */
#define WUNTRACED 2 /* tell about stopped, untraced children */
#define WAIT_ANY  (-1)  /* any process */
#define WAIT_MYPGRP 0 /* any process in my process group */

#define WSTOPPED  0177


typedef struct chldInfo {
   struct chldInfo *next;
   int pid;                     /* pid of terminated process */
   union wait status;           /* exit status of terminated process */
} chldInfo, *chldInfoPtr;

/*
 * The kernel doesn't currently use these macros, but if they ever get 
 * into the kernel sources, they'd really cause havoc; the numbers differ
 * between GNO and ORCA/C
 */
#undef SIGABRT
#undef SIGFPE
#undef SIGILL
#undef SIGINT
#undef SIGSEGV
#undef SIGTERM

#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt */
#define	SIGQUIT	3	/* quit */
#define	SIGILL	4	/* illegal instruction (not reset when caught) */
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#define	SIGABRT	6	/* abort() */
#define	SIGIOT	SIGABRT	/* compatibility */
#define	SIGEMT	7	/* EMT instruction */
#define	SIGFPE	8	/* floating point exception */
#define	SIGKILL	9	/* kill (cannot be caught or ignored) */
#define	SIGBUS	10	/* bus error */
#define	SIGSEGV	11	/* segmentation violation */
#define	SIGSYS	12	/* bad argument to system call */
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#define	SIGURG	16	/* urgent condition on IO channel */
#define	SIGSTOP	17	/* sendable stop signal not from tty */
#define	SIGTSTP	18	/* stop signal from tty */
#define	SIGCONT	19	/* continue a stopped process */
#define	SIGCHLD	20	/* to parent on child stop or exit */
#define SIGCLD	SIGCHLD	/* System V name for SIGCHLD */
#define	SIGTTIN	21	/* to readers pgrp upon background tty read */
#define	SIGTTOU	22	/* like TTIN for output if (tp->t_local&LTOSTOP) */
#define	SIGIO	23	/* input/output possible signal */
#define	SIGPOLL	SIGIO	/* System V name for SIGIO */
#define	SIGXCPU	24	/* exceeded CPU time limit */
#define	SIGXFSZ	25	/* exceeded file size limit */
#define	SIGVTALRM 26	/* virtual time alarm */
#define	SIGPROF	27	/* profiling time alarm */
#define SIGWINCH 28	/* window size changes */
#define SIGINFO	29	/* information request */
#define SIGUSR1 30	/* user defined signal 1 */
#define SIGUSR2 31	/* user defined signal 2 */

typedef void (*__SIG_FUNC__) (int, int);
#define SIG_DFL ((__SIG_FUNC__) (0))
#define SIG_IGN ((__SIG_FUNC__) (1))
#define SIG_ERR ((__SIG_FUNC__) (-1))


#define sigmask(m)  (1UL << ((unsigned)((m)-1)))

#endif
