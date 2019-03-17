#ifndef KERN_PORTS_H
#define KERN_PORTS_H

/*
 * Ports IPC
 *
 * This file is non-BSD.
 */



#define NPORTS		30		/* maximum number of ports 	*/
#define MAXMSGS		100		/* maximum messages on all ports*/
#define PTFREE		1    		/* port is free 		*/
#define PTLIMBO		2    		/* port is being deleted/reset 	*/
#define PTALLOC		3    		/* port is allocated 		*/
#define PTEMPTY		-1		/* initial semaphore entries 	*/
#define PTUNNAMED	NULL		/* port has not been bound	*/

struct  ptnode  {        		/* node on list of message ptrs */
	long int ptmsg;			/* a one-word message 		*/
        struct ptnode *ptnext;		/* address of next node on list */
};

struct  pt	{			/* entry in the port table	*/
	int	ptstate;		/* port state (FREE/LIMBO/ALLOC)*/
        char	*ptname;		/* name bound to port		*/
        int	ptssem;			/* sender semaphore 		*/
        int	ptrsem;			/* receiver semaphore		*/
        int	ptmaxcnt;		/* max messages to be queued	*/
        int	ptseq;			/* sequence changed at creation	*/
        struct	ptnode	*pthead;	/* list of message pointers	*/
        struct	ptnode	*pttail;        /* tail of message list		*/
};

extern	struct	ptnode	*ptfree;	/* list of free nodes		*/
extern	struct	pt	ports[];	/* port table			*/
extern	int	ptnextp;		/* next port to examine when	*/
					/*   looking for a free one	*/

#define isbadport(portid)	( (portid) < 0 || (portid)>=NPORTS )

#if 0
int	pcreate (int count);
int	psend (int portid, long msg);
long	preceive (int portid);
int	pdelete (int portid, int (*dispose)(long));
int	preset (int portid, int (*dispose)(long));
int	pbind (int portid, const char *name);
int	pgetport (const char *name);
int	pgetcount (int portid);
#endif

#endif