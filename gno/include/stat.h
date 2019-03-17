#ifndef KERN_STAT_H
#define KERN_STAT_H


typedef	char *		caddr_t;	/* core address */
typedef	long		daddr_t;	/* disk address */
typedef	unsigned short	dev_t;		/* device number */	/* non-BSD */
typedef unsigned long	fixpt_t;	/* fixed point number */
typedef	unsigned short	gid_t;		/* group id */
typedef	unsigned long	ino_t;		/* inode number */
typedef	unsigned short	mode_t;		/* permissions */
typedef	unsigned short	nlink_t;	/* link count */
typedef	long		off_t;		/* file offset */
typedef	short		pid_t;		/* process id */
typedef	long		segsz_t;	/* segment size */
typedef	long		swblk_t;	/* swap offset */
typedef	unsigned short	uid_t;		/* user id */

typedef long time_t;
struct stat
{
    dev_t               st_dev;
    ino_t               st_ino;
    unsigned short      st_mode;
    short               st_nlink;           /* number of links to file = 0 */
    uid_t               st_uid;             /* user id = 0 */
    gid_t               st_gid;             /* group id = 0 */
    dev_t               st_rdev;
    off_t               st_size;
    time_t              st_atime;           /* last access time = mod time */
    int                 st_spare1;
    time_t              st_mtime;
    int                 st_spare2;
    time_t              st_ctime;
    int                 st_spare3;
    long                st_blksize;
    long                st_blocks;
    long                st_spare4[2];
};

#define STAT_BSIZE      512       /* size of blocks in st_blocks */
#define STAT_BSHIFT     9         /* log2(STAT_BSIZE) */
#define btosb(bytes) ((unsigned)(bytes) >> STAT_BSHIFT)
#define sbtob(db)    ((unsigned)(db) << STAT_BSHIFT)



#define	S_ISUID	0004000			/* set user id on execution */
#define	S_ISGID	0002000			/* set group id on execution */
#define	S_ISTXT	0001000			/* sticky bit */

#define	S_IRWXU	0000700			/* RWX mask for owner */
#define	S_IRUSR	0000400			/* R for owner */
#define	S_IWUSR	0000200			/* W for owner */
#define	S_IXUSR	0000100			/* X for owner */

#define	S_IREAD		S_IRUSR
#define	S_IWRITE	S_IWUSR
#define	S_IEXEC		S_IXUSR

#define	S_IRWXG	0000070			/* RWX mask for group */
#define	S_IRGRP	0000040			/* R for group */
#define	S_IWGRP	0000020			/* W for group */
#define	S_IXGRP	0000010			/* X for group */

#define	S_IRWXO	0000007			/* RWX mask for other */
#define	S_IROTH	0000004			/* R for other */
#define	S_IWOTH	0000002			/* W for other */
#define	S_IXOTH	0000001			/* X for other */

#define	S_IFMT	 0170000		/* type of file mask */
#define	S_IFIFO	 0010000		/* named pipe (fifo) */
#define	S_IFCHR	 0020000		/* character special */
#define	S_IFDIR	 0040000		/* directory */
#define	S_IFBLK	 0060000		/* block special */
#define	S_IFREG	 0100000		/* regular */
#define	S_IFLNK	 0120000		/* symbolic link */
#define	S_IFSOCK 0140000		/* socket */
#define	S_ISVTX	 0001000		/* save swapped text even after use */

#define	S_ISDIR(m)	(((m) & 0170000) == 0040000)	/* directory */
#define	S_ISCHR(m)	(((m) & 0170000) == 0020000)	/* char special */
#define	S_ISBLK(m)	(((m) & 0170000) == 0060000)	/* block special */
#define	S_ISREG(m)	(((m) & 0170000) == 0100000)	/* regular file */
#define	S_ISFIFO(m)	(((m) & 0170000) == 0010000 || \
			 ((m) & 0170000) == 0140000)	/* fifo or socket */
#define	S_ISLNK(m)	(((m) & 0170000) == 0120000)	/* symbolic link */
#define	S_ISSOCK(m)	(((m) & 0170000) == 0010000 || \
			 ((m) & 0170000) == 0140000)	/* fifo or socket */

#define	ACCESSPERMS	(S_IRWXU|S_IRWXG|S_IRWXO)	/* 0777 */
							/* 7777 */
#define	ALLPERMS	(S_ISUID|S_ISGID|S_ISTXT|S_IRWXU|S_IRWXG|S_IRWXO)
							/* 0666 */
#define	DEFFILEMODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

#define S_BLKSIZE	512		/* block size used in the stat struct */


#endif
