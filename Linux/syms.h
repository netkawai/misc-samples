/* @(#)20	1.9  com/inc/syms.h, bos, bos320 4/19/91 16:31:08 */
#ifndef	_H_SYMS
#define _H_SYMS
/*
 * COMPONENT_NAME: (CMDLD) xcoff object file format definition
 *
 * FUNCTIONS: syms.h
 *
 * ORIGINS: 3, 27
 *
 * (C) COPYRIGHT International Business Machines Corp. 1988, 1989
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */

/*********************************************************************
 *
 *	COFF Extended Object File Format:
 *		syms.h
 *	Derived from AT&T UNIX System V Release 2.0 COFF
 *
 *	Structure similar to original COFF
 *
 **********************************************************************/

/*		Storage Classes are defined in storclass.h */
#include "storclass.h"

/*		Number of characters in a symbol name */
#define	SYMNMLEN	8
/*		Number of characters in a file name */
#define	FILNMLEN	14
/*		Number of array dimensions in auxiliary entry */
#define	DIMNUM		4

struct syment
{
	union
	{
		char		_n_name[SYMNMLEN];	/* old COFF version */
		struct
		{
			long	_n_zeroes;	/* new == 0 */
			long	_n_offset;	/* offset into string table */
		} _n_n;
		char		*_n_nptr[2];	/* allows for overlaying */
	} _n;
#ifndef pdp11
	unsigned
#endif
	long			n_value;	/* value of symbol */
	short			n_scnum;	/* section number */
	unsigned short		n_type;		/* type and derived type */
	char			n_sclass;	/* storage class */
	char			n_numaux;	/* number of aux. entries */
};

#define n_name		_n._n_name
#define n_nptr		_n._n_nptr[1]
#define n_zeroes	_n._n_n._n_zeroes
#define n_offset	_n._n_n._n_offset

/*
 * Relocatable symbols have a section number of the
 * section in which they are defined. Otherwise, section
 * numbers have the following meanings:
 */
	/* undefined symbol */
#define	N_UNDEF	0
	/* value of symbol is absolute */
#define	N_ABS		-1
	/* special debugging symbol -- value of symbol is meaningless */
#define	N_DEBUG	-2
#if	0	/* Should not need this ??? chh */
	/* indicates symbol needs transfer vector (preload) */
#define	N_TV		(unsigned short)-3

	/* indicates symbol needs transfer vector (postload) */

#define	P_TV		(unsigned short)-4
#endif

/*
 * The fundamental type of a symbol packed into the low
 * 4 bits of the word.
 */

#define	_EF	".ef"

#define	T_NULL		0
#define	T_ARG		1	/* function argument (only used by compiler) */
#define	T_CHAR		2	/* character */
#define	T_SHORT		3	/* short integer */
#define	T_INT		4	/* integer */
#define	T_LONG		5	/* long integer */
#define	T_FLOAT		6	/* floating point */
#define	T_DOUBLE	7	/* double word */
#define	T_STRUCT	8	/* structure */
#define	T_UNION		9	/* union */
#define	T_ENUM		10	/* enumeration */
#define	T_MOE		11	/* member of enumeration */
#define	T_UCHAR		12	/* unsigned character */
#define	T_USHORT	13	/* unsigned short */
#define	T_UINT		14	/* unsigned integer */
#define	T_ULONG		15	/* unsigned long */

/*
 * derived types are:
 */

#define DT_NON	0	/* no derived type */
#define DT_PTR	1	/* pointer */
#define DT_FCN	2	/* function */
#define DT_ARY	3	/* array */

/*
 *	type packing constants
 */

#define N_BTMASK	017
#define N_TMASK		060
#define N_TMASK1	0300
#define N_TMASK2	0360
#define N_BTSHFT	4
#define N_TSHIFT	2

#ifndef _COMPILER
/*
 *	MACROS
 */

	/* Basic Type of x */
#define BTYPE(x)	((x) & N_BTMASK)

	/* Is x a pointer? */
#define ISPTR(x)	(((x) & N_TMASK) == (DT_PTR << N_BTSHFT))

	/* Is x a function? */
#define ISFCN(x)	(((x) & N_TMASK) == (DT_FCN << N_BTSHFT))

	/* Is x an array? */
#define ISARY(x)	(((x) & N_TMASK) == (DT_ARY << N_BTSHFT))

	/* Is x a structure, union, or enumeration TAG? */
#define ISTAG(x)	((x)==C_STRTAG || (x)==C_UNTAG || (x)==C_ENTAG)

#define INCREF(x) ((((x)&~N_BTMASK)<<N_TSHIFT)|(DT_PTR<<N_BTSHFT)|(x&N_BTMASK))

#define DECREF(x) ((((x)>>N_TSHIFT)&~N_BTMASK)|((x)&N_BTMASK))
#endif

/*************************************************************************
 *
 *	AUXILIARY ENTRY FORMAT
 *
 *************************************************************************/

union auxent
{
	struct
	{
		long		x_tagndx;	/* str, un, or enum tag indx */
						/* exception table offset */
#define 		x_exptr	x_tagndx
		union
		{
			struct
			{
				unsigned short	x_lnno;	/* declaration line number */
				unsigned short	x_size;	/* str, union, array size */
			} x_lnsz;
			long	x_fsize;	/* size of function */
		} x_misc;
		union
		{
			struct			/* if ISFCN, tag, or .bb */
			{
				long	x_lnnoptr;	/* ptr to fcn line # */
				long	x_endndx;	/* entry ndx past block end */
			} x_fcn;
			struct			/* if ISARY, up to 4 dimen. */
			{
				unsigned short	x_dimen[DIMNUM];
			} x_ary;
		} x_fcnary;
		unsigned short	x_tvndx;		/* tv index */
	} x_sym;
	union
	{
		char	x_fname[FILNMLEN];
		struct
		{
			long		x_zeroes;
			long		x_offset;
			char		x_pad[FILNMLEN-8];
			unsigned char	x_ftype;
		} _x;
	} x_file;
	struct
	{
		long		x_scnlen; /* section length */
		unsigned short	x_nreloc; /* number of relocation entries */
		unsigned short	x_nlinno; /* number of line numbers */
	} x_scn;

#if	0	/* Don't think we need this ??? chh */
	struct
	{
		long		x_tvfill;	/* tv fill value */
		unsigned short	x_tvlen;	/* length of .tv */
		unsigned short	x_tvran[2];	/* tv range */
	} x_tv;	/* info about .tv section (in auxent of symbol .tv)) */
#endif

	struct
	{
		long		x_scnlen;	/* csect length */
		long		x_parmhash;	/* parm type hash index */
		unsigned short	x_snhash;	/* sect num with parm hash */
		unsigned char	x_smtyp;	/* symbol align and type */
						/* 0-4 - Log 2 of alignment */
						/* 5-7 - symbol type */
		unsigned char	x_smclas;	/* storage mapping class */
		long		x_stab;		/* dbx stab info index */
		unsigned short	x_snstab;	/* sect num with dbx stab */
	} x_csect; /* csect definition information */
};

/*	Defines for File auxiliary  definitions	*/
#define	XFT_FN	0	/* Source File Name */
#define	XFT_CT	1	/* Compile Time Stamp */
#define	XFT_CV	2	/* Compiler Version Number */
#define	XFT_CD	128	/* Compiler Defined Information */

/*	Defines for CSECT definitions	*/
/*	Symbol Type	*/
#define	XTY_ER	0	/* External Reference */
#define	XTY_SD	1	/* CSECT Section Definition */
#define XTY_LD	2	/* Entry Point - Label Definition */
#define XTY_CM	3	/* Common (BSS) */
#define XTY_EM	4	/* Error Message - Linkedit usage */
#define XTY_US	5	/* Unset */

/*	Storage Mapping Class	*/
/*		READ ONLY CLASSES */
#define	XMC_PR	0	/* Program Code */
#define	XMC_RO	1	/* Read Only Constant */
#define XMC_DB	2	/* Debug Dictionary Table */
#define XMC_GL	6	/* Global Linkage (Interfile Interface Code) */
#define XMC_XO	7	/* Extended Operation (Pseudo Machine Instruction */
#define XMC_SV	8	/* Supervisor Call */
#define XMC_TI	12	/* Traceback Index csect */
#define XMC_TB	13	/* Traceback table csect */

/*		READ WRITE CLASSES */
#define XMC_RW	5	/* Read Write Data */
#define XMC_TC0 15	/* TOC Anchor for TOC Addressability */
#define XMC_TC	3	/* General TOC Entry */
#define XMC_DS	10	/* Descriptor csect */
#define XMC_UA	4	/* Unclassified - Treated as Read Write */
#define XMC_BS	9	/* BSS class (uninitialized static internal) */
#define XMC_UC	11	/* Un-named Fortran Common */

#define	SYMENT	struct syment
#define	SYMESZ	18	/* sizeof(SYMENT) */

#define	AUXENT	union auxent
#define	AUXESZ	18	/* sizeof(AUXENT) */

/*	Defines for "special" symbols */

#if vax
#define _ETEXT	"_etext"
#define	_EDATA	"_edata"
#define	_END	"_end"
#else
#define _ETEXT	"etext"
#define _EDATA	"edata"
#define _END	"end"
#endif

#define _START	"_start"
#endif /* _H_SYMS */
