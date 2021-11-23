	.arch armv8-a
	.file	"loop_test.c"
	.text
	.align	2
	.p2align 3,,7
	.global	test
	.type	test, %function
test:
.LFB0:
	.cfi_startproc
	mov	w0, 45
	ret
	.cfi_endproc
.LFE0:
	.size	test, .-test
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
