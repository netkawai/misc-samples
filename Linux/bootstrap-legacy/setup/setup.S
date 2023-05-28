#define CR0_PE_ON	0x1
#define CRO_PE_OFF	0xfffffffe
#define PROT_MODE_CSEG	0x8
#define PROT_MODE_DSEG	0x10
#define PSEUDO_RM_CSEG	0x18
#define PSEUDO_RM_DSEG	0x20
#define STACKOFF	0x2000 - 0x10
#define PROTSTACKINIT	0x68000 - 0x10
	
	.file "setup.S"
	.text
	.code16
.globl	_start			/* ld */
_start:				/*  */
	
	xorw	%ax,%ax		/* */
	movw	%ax,%ds		/*  */
	movw	%ax,%es		/*  */
	movw	%ax,%ss
	
	movl	$STACKOFF,%ebp	/* */
	movl	%ebp,%esp
	sti			/* */
	data32
	call	real_to_proto
	
	.code32
	call	_main
	
	call	proto_to_real
	.code16
	movw	$0x61,%dx
	movb	$0x03,%al
	outb	%al,%dx
1:	
	jmp	1b


	.code32
/*  */
.globl	putchar	; putchar:
	pushl	%ebp
	pushl	%eax
	pushl	%ebx
	
	movb	16(%esp),%bl	/* */
	cmpb	$0xa,%bl
	jne	pc_notnewline

	pushl	$0xd
	call	putchar
	popl	%eax

pc_notnewline:
	call	proto_to_real
	.code16

	movb	%bl,%al
	movb	$0xe,%ah
	movw	$1,%bx
	int	$0x10

	data32
	call	real_to_proto
	.code32

	pop	%ebx
	pop	%eax
	pop	%ebp
	ret

	.code32
/* Real Mode interrupt */
.globl intx		; intx :
	pushl	%ebp

	movl	%esp,%ebp	/* (%ebp),(return-address) */
	pushl	%ebx
	
	movb	8(%ebp),%bl
	
	movl	%ebx,%eax
	popl	%ebx
	popl	%ebp
	ret
	
	.code32
/*  */
.globl	real_to_proto	; real_to_proto :
	.code16		
	/* GDT$N%m!<%I*/
	data32
	addr32
	lgdt	gdtdesc

	/*  */
	movl	%cr0,%eax
	orl	$CR0_PE_ON,%eax
	movl	%eax,%cr0
	
	/*  */
	data32
	ljmp	$PROT_MODE_CSEG,$protcseg

	/* */
	.code32	
protcseg:
	/* */
	movw	$PROT_MODE_DSEG,%ax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movw	%ax,%ss

	movl	(%esp),%eax
	movl	%eax,STACKOFF

	movl	protstack,%eax
	movl	%eax,%esp
	movl	%eax,%ebp
	
	movl	STACKOFF,%eax
	movl	%eax,(%esp)

	xorl	%eax,%eax
	ret

/*  */
	.code32
.global	proto_to_real
	.type	proto_to_real,@function
proto_to_real:
	/*  */
	lgdt	gdtdesc

	/*  */
	movl	%esp,%eax
	movl	%eax,protstack

	/* */
	movl	(%esp),%eax
	movl	%eax,STACKOFF

	/*  */
	movl	$STACKOFF,%eax
	movl	%eax,%esp
	movl	%eax,%ebp

	/* */
	movl	$PSEUDO_RM_DSEG,%ax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movw	%ax,%ss

	ljmp	$PSEUDO_RM_CSEG,$tmpcseg

tmpcseg:
	.code16

	movl	%cr0,%eax
	andl	$CRO_PE_OFF,%eax
	movl	%eax,%cr0

	data32
	ljmp	$0,$realcseg

realcseg:
	xorl	%eax,%eax

	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movw	%ax,%ss
	
	data32
	ret


/*  */
	.p2align	2
protstack:
	.long	PROTSTACKINIT

/* GDT Table */
	.p2align	2
gdt:
	.word	0,0
	.byte	0,0,0,0

	/* code segment */
	.word 0xFFFF,0
	.byte 0, 0x9A, 0xCF, 0

	/* data segment */
	.word 0xFFFF,0
	.byte 0, 0x92, 0xCF, 0

	/* 16 bit real mode CS */
	.word 0xFFFF,0
	.byte 0, 0x9E, 0, 0

	/* 16 bit real mode DS */
	.word 0xFFFF,0
	.byte 0,0x92,0,0

gdtdesc:
	.word	0x27	/* limit */
	.long	gdt	/* addr*/