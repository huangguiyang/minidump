/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Id$ */

/*
 * hgy: Modified so it can be complied & run correctly in both minix and 64 bit linux.
 */

#ifndef __OUT_H_INCLUDED
#define __OUT_H_INCLUDED

#ifdef _MINIX
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
#else
#include <stdint.h>
#endif

/*
 * output format for ACK assemblers
 */

struct outhead {
	uint16_t oh_magic;	/* magic number */
	uint16_t oh_stamp;	/* version stamp */
	uint16_t oh_flags;	/* several format flags */
	uint16_t oh_nsect;	/* number of outsect structures */
	uint16_t oh_nrelo;	/* number of outrelo structures */
	uint16_t oh_nname;	/* number of outname structures */
	uint32_t oh_nemit;		/* sum of all os_flen */
	uint32_t oh_nchar;		/* size of string area */
};

#define O_MAGIC	0x0201			/* magic number of output file */
#define	O_STAMP	0			/* version stamp */
#define MAXSECT	64			/* Maximum number of sections */

#define	HF_LINK	0x0004			/* unresolved references left */
#define	HF_8086	0x0008			/* os_base specially encoded */

struct outsect {
	uint32_t os_base;		/* startaddress in machine */
	uint32_t os_size;		/* section size in machine */
	uint32_t os_foff;		/* startaddress in file */
	uint32_t os_flen;		/* section size in file */
	uint32_t os_lign;		/* section alignment */
};

struct outrelo {
	uint8_t or_type;		/* type of reference */
	uint8_t or_sect;		/* referencing section */
	uint16_t or_nami;       /* referenced symbol index */
	uint32_t or_addr;		/* referencing address */
};

struct outname {
	union {
	  int32_t	on_ptr;		/* symbol name (in core) */
	  int32_t	on_off;			/* symbol name (in file) */
	}	on_u;
#define on_mptr	on_u.on_ptr
#define on_foff	on_u.on_off
	uint16_t on_type;		/* symbol type */
	uint16_t on_desc;		/* debug info */
	uint32_t on_valu;		/* symbol value */
};

/*
 * relocation type bits
 */
#define RELSZ	0x07		/* relocation length */
#define RELO1	   1		/* 1 byte */
#define RELO2	   2		/* 2 bytes */
#define RELO4	   4		/* 4 bytes */
#define RELPC	0x08		/* pc relative */
#define RELBR	0x10		/* High order byte lowest address. */
#define RELWR	0x20		/* High order word lowest address. */

/*
 * section type bits and fields
 */
#define S_TYP	0x007F		/* undefined, absolute or relative */
#define S_EXT	0x0080		/* external flag */
#define S_ETC	0x7F00		/* for symbolic debug, bypassing 'as' */

/*
 * S_TYP field values
 */
#define S_UND	0x0000		/* undefined item */
#define S_ABS	0x0001		/* absolute item */
#define S_MIN	0x0002		/* first user section */
#define S_MAX	(S_TYP-1)	/* last user section */
#define S_CRS	S_TYP		/* on_valu is symbol index which contains value */

/*
 * S_ETC field values
 */
#define S_SCT	0x0100		/* section names */
#define S_LIN	0x0200		/* hll source line item */
#define S_FIL	0x0300		/* hll source file item */
#define S_MOD	0x0400		/* ass source file item */
#define S_COM	0x1000		/* Common name. */
#define S_STB	0xe000		/* entries with any of these bits set are
				   reserved for debuggers
				*/

/*
 * structure format strings
 */
#define SF_HEAD		"22222244"
#define SF_SECT		"44444"
#define SF_RELO		"1124"
#define SF_NAME		"4224"

/*
 * structure sizes (bytes in file; add digits in SF_*)
 */
#define SZ_HEAD		20
#define SZ_SECT		20
#define SZ_RELO		8
#define SZ_NAME		12

/*
 * file access macros
 */
#define BADMAGIC(x)	((x).oh_magic!=O_MAGIC)
#define OFF_SECT(x)	SZ_HEAD
#define OFF_EMIT(x)	(OFF_SECT(x) + ((int32_t)(x).oh_nsect * SZ_SECT))
#define OFF_RELO(x)	(OFF_EMIT(x) + (x).oh_nemit)
#define OFF_NAME(x)	(OFF_RELO(x) + ((int32_t)(x).oh_nrelo * SZ_RELO))
#define OFF_CHAR(x)	(OFF_NAME(x) + ((int32_t)(x).oh_nname * SZ_NAME))

#endif /* __OUT_H_INCLUDED */
