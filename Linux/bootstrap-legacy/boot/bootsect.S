/* First Boot Program */
	
	.file "bootsect.S"
			
	.text
	.code16
.globl _start		/* entry point */
_start:			/*  */
	cli			/*  */
	jmp	restart
restart:	
	movw	$0x07c0,%ax	/*  */
	movw	%ax,%ds		/*  */
	movw	$0x9000,%ax
	movw	%ax,%es
	movw	$256,%cx
	subw	%si,%si
	subw	%di,%di
	cld
	rep
	movsw			
	ljmp	$0x9000,$go	/*  */
				/*  */
go:
	movw	$0x9000,%ax	/* */
	movw	%ax,%es
	movw	%ax,%ss
	movw	%ax,%ds
	movw	$2048,%sp
	sti			/*  */
	movw	$0x61,%dx	/* PIC */
	movb	$0x03,%al
	outb	%al,%dx
	movw	$message,%si	/* Boot Message*/
	call	print
	call	put_return
		
	call	init_disk	/* FD */
	call	diskread	/*  */
	
	ljmp	$0x0,$0x1000	/*  */
				/*(0x0100:0x0000=0x01000=0x0:0x1000) */

init_disk:	
	movb	$0,%ah		/* BIOS:FD */
	movb	$0,%dl
	int	$0x13
	ret
	
diskread:
	movw	$0x0100,%ax
	movw	%ax,%es
	movb	$0x02,%ah	/* BIOS: */
	movb	$2,%al		/*  */
	movw	$0x0,%bx	/* 0x0100:0x0000 */
	movw	$0x0002,%cx	/*  */
	movw	$0x0000,%dx	/* */
	int	$0x13
	ret
			
/* 
	input:	bx=
	output:	
*/
1:
	movw	$0x0001,%bx
	movb	$0xe,%ah
	int 	$0x10

	incw	%si
print:
	movb	(%si),%al
	cmpb	$0,%al
	jne	1b
	ret
/*  */
put_return:
	movb	$13,%al
	movw	$0x0001,%bx
	movb	$0xe,%ah
	int	$0x10
	movb	$10,%al
	int	$0x10
	ret
	
message: .string	"BootSect Stage"
	 .byte		13,10
			
	. = 510
/* */
boot_flag:
	.word 0xaa55