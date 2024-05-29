	.file	"lab1.cpp"
	.text
	.p2align 4
	.globl	_Z1fiii
	.type	_Z1fiii, @function
_Z1fiii:
.LFB1819:
	.cfi_startproc
	endbr64
	cmpl	%esi, Sx(%rip)
	pxor	%xmm0, %xmm0
	jne	.L6
	cmpl	%edi, Sy(%rip)
	je	.L10
.L6:
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	pxor	%xmm1, %xmm1
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	cvtsi2sdl	%edx, %xmm1
	mulsd	t(%rip), %xmm1
	subsd	.LC1(%rip), %xmm1
	mulsd	.LC2(%rip), %xmm1
	movapd	%xmm1, %xmm0
	movsd	%xmm1, 8(%rsp)
	mulsd	%xmm1, %xmm0
	mulsd	.LC3(%rip), %xmm0
	call	exp@PLT
	movsd	8(%rsp), %xmm1
	movsd	%xmm0, (%rsp)
	movapd	%xmm1, %xmm0
	call	sin@PLT
	mulsd	(%rsp), %xmm0
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1819:
	.size	_Z1fiii, .-_Z1fiii
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC16:
	.string	"%lf\n"
.LC17:
	.string	"wb"
.LC18:
	.string	"file.dat"
.LC20:
	.string	"\n %lf"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB1820:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pxor	%xmm0, %xmm0
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$424, %rsp
	.cfi_def_cfa_offset 480
	movl	Nx(%rip), %ebp
	movsd	.LC7(%rip), %xmm1
	movq	%fs:40, %rax
	movq	%rax, 408(%rsp)
	xorl	%eax, %eax
	movl	Ny(%rip), %ebx
	leal	-1(%rbp), %eax
	movapd	%xmm1, %xmm2
	cvtsi2sdl	%eax, %xmm0
	movl	%eax, 24(%rsp)
	leal	-1(%rbx), %eax
	divsd	%xmm0, %xmm2
	pxor	%xmm0, %xmm0
	cvtsi2sdl	%eax, %xmm0
	divsd	%xmm0, %xmm1
	movsd	%xmm2, 8(%rsp)
	movsd	%xmm1, 16(%rsp)
	cmpl	$1000, %ebp
	jg	.L71
	cmpl	$1000, %ebx
	jle	.L61
.L71:
	movsd	.LC5(%rip), %xmm0
.L12:
	movslq	%ebp, %rax
	movslq	%ebx, %rdi
	movl	%ebp, %r15d
	movsd	%xmm0, t(%rip)
	imulq	%rax, %rdi
	movq	%rax, (%rsp)
	imull	%ebx, %r15d
	salq	$3, %rdi
	call	malloc@PLT
	movslq	%r15d, %r15
	movq	%rax, 312(%rsp)
	salq	$3, %r15
	testl	%ebx, %ebx
	jle	.L15
	movl	%ebp, %r13d
	sarl	%r13d
	testl	%ebp, %ebp
	jle	.L15
	cmpl	%r13d, %ebp
	movl	%r13d, %r8d
	movl	$1, %r11d
	movq	%rax, %rdi
	cmovle	%ebp, %r8d
	movq	%r15, 32(%rsp)
	movsd	.LC13(%rip), %xmm3
	movl	$1, %r14d
	movapd	.LC12(%rip), %xmm6
	movsd	.LC11(%rip), %xmm7
	testl	%r8d, %r8d
	movdqa	.LC8(%rip), %xmm5
	movapd	.LC9(%rip), %xmm2
	cmovg	%r8d, %r11d
	movdqa	.LC10(%rip), %xmm4
	xorl	%ecx, %ecx
	xorl	%r9d, %r9d
	movl	%r11d, %r12d
	movl	%r11d, %r10d
	shrl	$2, %r12d
	andl	$-4, %r10d
	salq	$5, %r12
	.p2align 4,,10
	.p2align 3
.L21:
	testl	%r13d, %r13d
	jle	.L62
	cmpl	$4, %r8d
	jle	.L63
	movslq	%ecx, %rax
	movdqa	.LC6(%rip), %xmm1
	leaq	(%rdi,%rax,8), %rax
	leaq	(%rax,%r12), %rdx
	.p2align 4,,10
	.p2align 3
.L18:
	movdqa	%xmm1, %xmm0
	movups	%xmm2, (%rax)
	addq	$32, %rax
	paddd	%xmm5, %xmm1
	movups	%xmm2, -16(%rax)
	paddd	%xmm4, %xmm0
	cmpq	%rdx, %rax
	jne	.L18
	pshufd	$255, %xmm0, %xmm0
	movl	%r10d, %edx
	movd	%xmm0, %eax
	cmpl	%r11d, %r10d
	je	.L22
.L17:
	leal	(%rcx,%rdx), %eax
	cltq
	movsd	%xmm7, (%rdi,%rax,8)
	leal	1(%rdx), %eax
	cmpl	%r8d, %eax
	jge	.L22
	addl	%ecx, %eax
	cltq
	movsd	%xmm7, (%rdi,%rax,8)
	leal	2(%rdx), %eax
	cmpl	%r8d, %eax
	jge	.L22
	addl	%ecx, %eax
	cltq
	movsd	%xmm7, (%rdi,%rax,8)
	leal	3(%rdx), %eax
	cmpl	%r8d, %eax
	jge	.L22
	addl	%ecx, %eax
	cltq
	movsd	%xmm7, (%rdi,%rax,8)
	leal	4(%rdx), %eax
.L22:
	cmpl	%eax, %ebp
	jle	.L26
.L16:
	movl	%ebp, %edx
	movl	%r14d, %r15d
	subl	%eax, %edx
	cmpl	%eax, %ebp
	cmovg	%edx, %r15d
	cmpl	$1, %edx
	je	.L23
	cmpl	%eax, %ebp
	jle	.L23
	movslq	%ecx, %rsi
	movslq	%eax, %rdx
	addq	%rsi, %rdx
	movl	%r15d, %esi
	shrl	%esi
	leaq	(%rdi,%rdx,8), %rdx
	salq	$4, %rsi
	addq	%rdx, %rsi
	.p2align 4,,10
	.p2align 3
.L25:
	movups	%xmm6, (%rdx)
	addq	$16, %rdx
	cmpq	%rsi, %rdx
	jne	.L25
	movl	%r15d, %edx
	andl	$-2, %edx
	addl	%edx, %eax
	cmpl	%r15d, %edx
	je	.L26
.L23:
	addl	%ecx, %eax
	cltq
	movsd	%xmm3, (%rdi,%rax,8)
.L26:
	addl	$1, %r9d
	addl	%ebp, %ecx
	cmpl	%r9d, %ebx
	jne	.L21
	movq	32(%rsp), %r15
	movl	$16, %edi
	call	malloc@PLT
	movq	%r15, %rdi
	movq	%rax, %rbp
	movq	%rax, 304(%rsp)
	call	malloc@PLT
	movq	%r15, %rdi
	xorl	%r15d, %r15d
	movq	%rax, 0(%rbp)
	movq	%rax, %r14
	call	malloc@PLT
	movq	%rbp, %rcx
	movq	(%rsp), %r12
	movq	%rax, 360(%rsp)
	movq	%rax, %rbp
	movq	%rax, 8(%rcx)
	movl	24(%rsp), %eax
	salq	$3, %r12
	leaq	8(,%rax,8), %r13
	.p2align 4,,10
	.p2align 3
.L28:
	movq	%r14, %rdi
	movq	%r13, %rdx
	xorl	%esi, %esi
	addl	$1, %r15d
	call	memset@PLT
	movq	%rbp, %rdi
	movq	%r13, %rdx
	xorl	%esi, %esi
	call	memset@PLT
	addq	%r12, %r14
	addq	%r12, %rbp
	cmpl	%r15d, %ebx
	jne	.L28
.L58:
	movslq	Nt(%rip), %rdi
	movq	%rdi, %rbx
	salq	$3, %rdi
	call	malloc@PLT
	movq	%rax, 352(%rsp)
	testl	%ebx, %ebx
	jle	.L32
	movq	%rax, %rdi
	leal	-1(%rbx), %eax
	xorl	%esi, %esi
	leaq	8(,%rax,8), %rdx
	call	memset@PLT
.L32:
	movsd	8(%rsp), %xmm0
	leaq	368(%rsp), %rsi
	movl	$4, %edi
	movsd	.LC14(%rip), %xmm10
	mulsd	%xmm0, %xmm0
	movapd	%xmm10, %xmm12
	divsd	%xmm0, %xmm12
	movsd	16(%rsp), %xmm0
	mulsd	%xmm0, %xmm0
	divsd	%xmm0, %xmm10
	movsd	%xmm12, 8(%rsp)
	movsd	%xmm10, (%rsp)
	call	clock_gettime@PLT
	movl	Nt(%rip), %eax
	movsd	(%rsp), %xmm10
	movsd	8(%rsp), %xmm12
	cmpl	$1, %eax
	movl	%eax, 344(%rsp)
	jle	.L30
	movsd	t(%rip), %xmm13
	movl	Nx(%rip), %eax
	movl	Sy(%rip), %ecx
	movl	Ny(%rip), %edx
	movsd	%xmm13, 320(%rsp)
	movl	Sx(%rip), %r11d
	mulsd	%xmm13, %xmm13
	leal	-1(%rax), %r12d
	movl	%ecx, (%rsp)
	cmpl	$2, %edx
	jle	.L30
	cmpl	$1, %r12d
	jle	.L30
	movq	352(%rsp), %rcx
	movslq	%eax, %r13
	movl	%r12d, %r9d
	movq	312(%rsp), %rsi
	movq	%r13, 136(%rsp)
	movq	.LC19(%rip), %xmm11
	leaq	8(%rcx), %rbp
	leaq	0(,%r13,8), %rcx
	addq	%rcx, %rsi
	movq	%rcx, 200(%rsp)
	leal	(%rax,%rax), %ecx
	movq	%rbp, %r10
	movslq	%ecx, %rcx
	movq	%rsi, 328(%rsp)
	leal	-3(%rdx), %esi
	salq	$3, %rcx
	movl	%esi, 348(%rsp)
	movq	%rcx, 336(%rsp)
	leal	-2(%rax), %ecx
	cmpl	$2, %ecx
	movl	%ecx, 296(%rsp)
	setg	%cl
	cmpl	$-2147483647, %eax
	setne	%al
	xorl	%r15d, %r15d
	andl	%eax, %ecx
	movq	%r15, %r14
	movl	$1, %eax
	movb	%cl, 231(%rsp)
	.p2align 4,,10
	.p2align 3
.L39:
	movl	348(%rsp), %edx
	pxor	%xmm0, %xmm0
	movq	$0, 144(%rsp)
	cvtsi2sdl	%eax, %xmm0
	movq	312(%rsp), %r13
	mulsd	320(%rsp), %xmm0
	movq	$0, 192(%rsp)
	leaq	2(%rdx), %rsi
	subsd	.LC1(%rip), %xmm0
	mulsd	.LC2(%rip), %xmm0
	movq	$1, 8(%rsp)
	movq	%rsi, 216(%rsp)
	leal	1(%rax), %esi
	andl	$1, %eax
	movq	328(%rsp), %r12
	movl	%esi, %edx
	movl	%esi, 300(%rsp)
	movq	304(%rsp), %rsi
	andl	$1, %edx
	movq	(%rsi,%rax,8), %rax
	movq	(%rsi,%rdx,8), %rcx
	movsd	%xmm0, 160(%rsp)
	mulsd	%xmm0, %xmm0
	movq	%rax, 208(%rsp)
	movq	136(%rsp), %rax
	mulsd	.LC3(%rip), %xmm0
	movq	%rcx, 176(%rsp)
	movq	%rax, 152(%rsp)
	movq	336(%rsp), %rax
	movq	%rax, 184(%rsp)
	movq	200(%rsp), %rax
	movq	%rax, 128(%rsp)
	movsd	%xmm0, 168(%rsp)
	.p2align 4,,10
	.p2align 3
.L36:
	cmpb	$0, 231(%rsp)
	movsd	(%r10), %xmm2
	je	.L87
	movq	128(%rsp), %rsi
	movsd	0(%r13), %xmm4
	movl	%r9d, 24(%rsp)
	movl	$1, %ebx
	movq	176(%rsp), %rbp
	movsd	(%r12), %xmm3
	movq	208(%rsp), %rax
	movapd	%xmm4, %xmm1
	movl	8(%rsp), %ecx
	addq	%rsi, %rbp
	addsd	%xmm3, %xmm1
	movq	144(%rsp), %rdi
	subq	152(%rsp), %rdi
	addq	%rsi, %rax
	movq	%rbp, 32(%rsp)
	movq	%rbp, %rdx
	movq	%rax, 40(%rsp)
	movq	%rax, %rsi
	movq	136(%rsp), %rax
	movl	%ecx, 16(%rsp)
	movq	%r12, %rcx
.L48:
	movsd	8(%rdx), %xmm14
	pxor	%xmm9, %xmm9
	movapd	%xmm14, %xmm0
	addsd	%xmm14, %xmm0
	movapd	%xmm0, %xmm6
	subsd	8(%rsi), %xmm6
	cmpl	%ebx, %r11d
	jne	.L40
	movl	16(%rsp), %r9d
	cmpl	%r9d, (%rsp)
	jne	.L40
	movsd	168(%rsp), %xmm0
	movq	%r10, 272(%rsp)
	movq	%rax, 264(%rsp)
	movq	%rdi, 256(%rsp)
	movq	%rcx, 248(%rsp)
	movq	%rsi, 240(%rsp)
	movq	%rdx, 232(%rsp)
	movl	%r11d, 88(%rsp)
	movsd	%xmm1, 280(%rsp)
	movsd	%xmm2, 120(%rsp)
	movsd	%xmm4, 112(%rsp)
	movsd	%xmm3, 104(%rsp)
	movsd	%xmm13, 96(%rsp)
	movsd	%xmm10, 80(%rsp)
	movsd	%xmm12, 72(%rsp)
	movsd	%xmm6, 64(%rsp)
	movsd	%xmm14, 56(%rsp)
	call	exp@PLT
	movsd	%xmm0, 48(%rsp)
	movsd	160(%rsp), %xmm0
	call	sin@PLT
	movsd	48(%rsp), %xmm9
	movsd	280(%rsp), %xmm1
	movq	272(%rsp), %r10
	movq	264(%rsp), %rax
	mulsd	%xmm0, %xmm9
	movq	256(%rsp), %rdi
	movq	248(%rsp), %rcx
	movq	240(%rsp), %rsi
	movq	232(%rsp), %rdx
	movsd	120(%rsp), %xmm2
	movsd	112(%rsp), %xmm4
	movsd	104(%rsp), %xmm3
	movsd	96(%rsp), %xmm13
	movl	88(%rsp), %r11d
	movsd	80(%rsp), %xmm10
	movsd	72(%rsp), %xmm12
	movsd	64(%rsp), %xmm6
	movq	.LC19(%rip), %xmm11
	movsd	56(%rsp), %xmm14
	.p2align 4,,10
	.p2align 3
.L40:
	movsd	8(%rcx), %xmm5
	movsd	8(%rdx,%rax,8), %xmm0
	movsd	8(%rcx,%rdi,8), %xmm8
	subsd	%xmm14, %xmm0
	addsd	%xmm5, %xmm3
	addsd	%xmm8, %xmm4
	movapd	%xmm8, %xmm7
	addsd	%xmm5, %xmm7
	mulsd	%xmm0, %xmm3
	movsd	8(%rdx,%rdi,8), %xmm0
	subsd	%xmm14, %xmm0
	mulsd	%xmm4, %xmm0
	addsd	%xmm3, %xmm0
	movsd	(%rdx), %xmm3
	subsd	%xmm14, %xmm3
	mulsd	%xmm10, %xmm0
	mulsd	%xmm3, %xmm1
	movsd	16(%rdx), %xmm3
	subsd	%xmm14, %xmm3
	mulsd	%xmm7, %xmm3
	addsd	%xmm3, %xmm1
	mulsd	%xmm12, %xmm1
	addsd	%xmm1, %xmm0
	addsd	%xmm9, %xmm0
	mulsd	%xmm13, %xmm0
	addsd	%xmm6, %xmm0
	movsd	%xmm0, 8(%rsi)
	andpd	%xmm11, %xmm0
	comisd	%xmm2, %xmm0
	jbe	.L41
	movsd	%xmm0, (%r10)
	movapd	%xmm0, %xmm2
.L41:
	movsd	16(%rdx), %xmm9
	leal	1(%rbx), %r9d
	movl	%ebx, %r8d
	pxor	%xmm14, %xmm14
	movapd	%xmm9, %xmm0
	addsd	%xmm9, %xmm0
	movapd	%xmm0, %xmm6
	subsd	16(%rsi), %xmm6
	cmpl	%r9d, %r11d
	jne	.L44
	movl	16(%rsp), %ebp
	cmpl	%ebp, (%rsp)
	jne	.L44
	movsd	168(%rsp), %xmm0
	movq	%r10, 288(%rsp)
	movq	%rax, 280(%rsp)
	movq	%rdi, 272(%rsp)
	movq	%rcx, 264(%rsp)
	movq	%rsi, 256(%rsp)
	movq	%rdx, 248(%rsp)
	movl	%r11d, 120(%rsp)
	movsd	%xmm2, 240(%rsp)
	movsd	%xmm13, 232(%rsp)
	movsd	%xmm10, 112(%rsp)
	movsd	%xmm12, 104(%rsp)
	movsd	%xmm9, 88(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm7, 72(%rsp)
	movsd	%xmm5, 64(%rsp)
	movsd	%xmm8, 56(%rsp)
	movl	%ebx, 96(%rsp)
	call	exp@PLT
	movsd	%xmm0, 48(%rsp)
	movsd	160(%rsp), %xmm0
	call	sin@PLT
	movsd	48(%rsp), %xmm14
	movq	288(%rsp), %r10
	movq	280(%rsp), %rax
	movq	272(%rsp), %rdi
	mulsd	%xmm0, %xmm14
	movq	264(%rsp), %rcx
	movq	256(%rsp), %rsi
	movq	248(%rsp), %rdx
	movl	120(%rsp), %r11d
	movsd	240(%rsp), %xmm2
	movsd	112(%rsp), %xmm10
	movsd	232(%rsp), %xmm13
	movsd	104(%rsp), %xmm12
	movq	.LC19(%rip), %xmm11
	movl	96(%rsp), %r8d
	movsd	88(%rsp), %xmm9
	movsd	80(%rsp), %xmm6
	movsd	72(%rsp), %xmm7
	movsd	64(%rsp), %xmm5
	movsd	56(%rsp), %xmm8
	.p2align 4,,10
	.p2align 3
.L44:
	movsd	16(%rcx,%rdi,8), %xmm4
	movsd	16(%rdx,%rdi,8), %xmm0
	movsd	16(%rcx), %xmm3
	subsd	%xmm9, %xmm0
	addsd	%xmm4, %xmm8
	addsd	%xmm3, %xmm5
	movapd	%xmm3, %xmm1
	addsd	%xmm4, %xmm1
	mulsd	%xmm0, %xmm8
	movsd	16(%rdx,%rax,8), %xmm0
	subsd	%xmm9, %xmm0
	mulsd	%xmm5, %xmm0
	movsd	24(%rdx), %xmm5
	subsd	%xmm9, %xmm5
	addsd	%xmm8, %xmm0
	movsd	8(%rdx), %xmm8
	mulsd	%xmm1, %xmm5
	subsd	%xmm9, %xmm8
	mulsd	%xmm10, %xmm0
	mulsd	%xmm8, %xmm7
	addsd	%xmm5, %xmm7
	mulsd	%xmm12, %xmm7
	addsd	%xmm7, %xmm0
	addsd	%xmm14, %xmm0
	mulsd	%xmm13, %xmm0
	addsd	%xmm6, %xmm0
	movsd	%xmm0, 16(%rsi)
	andpd	%xmm11, %xmm0
	comisd	%xmm2, %xmm0
	jbe	.L46
	movsd	%xmm0, (%r10)
	movapd	%xmm0, %xmm2
.L46:
	addl	$2, %ebx
	addq	$16, %rdx
	addq	$16, %rsi
	addq	$16, %rcx
	addl	$3, %r8d
	cmpl	%r8d, 296(%rsp)
	jg	.L48
	movl	24(%rsp), %r9d
	movq	32(%rsp), %rbp
	movq	40(%rsp), %rax
.L38:
	movq	176(%rsp), %rdx
	movq	184(%rsp), %rdi
	movslq	%ebx, %rbx
	movq	192(%rsp), %rcx
	movl	8(%rsp), %esi
	addq	%rdx, %rcx
	addq	%rdi, %rdx
	movq	%r12, %rdi
	movl	%r11d, %r12d
	.p2align 4,,10
	.p2align 3
.L53:
	movsd	0(%rbp,%rbx,8), %xmm4
	movq	%r15, %xmm5
	movapd	%xmm4, %xmm0
	addsd	%xmm4, %xmm0
	movapd	%xmm0, %xmm3
	subsd	(%rax,%rbx,8), %xmm3
	cmpl	%ebx, %r12d
	jne	.L49
	movq	%r14, %xmm5
	cmpl	%esi, (%rsp)
	jne	.L49
	movsd	168(%rsp), %xmm0
	movq	%rcx, 120(%rsp)
	movq	%rax, 112(%rsp)
	movq	%rdx, 104(%rsp)
	movq	%r10, 96(%rsp)
	movl	%esi, 88(%rsp)
	movq	%rdi, 80(%rsp)
	movl	%r9d, 56(%rsp)
	movsd	%xmm2, 72(%rsp)
	movsd	%xmm13, 64(%rsp)
	movsd	%xmm10, 48(%rsp)
	movsd	%xmm12, 40(%rsp)
	movsd	%xmm4, 32(%rsp)
	movsd	%xmm3, 24(%rsp)
	call	exp@PLT
	movsd	%xmm0, 16(%rsp)
	movsd	160(%rsp), %xmm0
	call	sin@PLT
	movsd	16(%rsp), %xmm5
	movq	120(%rsp), %rcx
	movq	112(%rsp), %rax
	movq	104(%rsp), %rdx
	mulsd	%xmm0, %xmm5
	movq	96(%rsp), %r10
	movl	88(%rsp), %esi
	movq	80(%rsp), %rdi
	movsd	72(%rsp), %xmm2
	movsd	64(%rsp), %xmm13
	movl	56(%rsp), %r9d
	movsd	48(%rsp), %xmm10
	movsd	40(%rsp), %xmm12
	movq	.LC19(%rip), %xmm11
	movsd	32(%rsp), %xmm4
	movsd	24(%rsp), %xmm3
	.p2align 4,,10
	.p2align 3
.L49:
	movsd	-8(%r13,%rbx,8), %xmm14
	movsd	0(%r13,%rbx,8), %xmm8
	movsd	(%rcx,%rbx,8), %xmm0
	movsd	-8(%rdi,%rbx,8), %xmm9
	movapd	%xmm14, %xmm6
	movsd	(%rdi,%rbx,8), %xmm7
	subsd	%xmm4, %xmm0
	addsd	%xmm8, %xmm6
	movapd	%xmm9, %xmm1
	addsd	%xmm7, %xmm1
	addsd	%xmm14, %xmm9
	addsd	%xmm8, %xmm7
	mulsd	%xmm6, %xmm0
	movsd	(%rdx,%rbx,8), %xmm6
	subsd	%xmm4, %xmm6
	mulsd	%xmm1, %xmm6
	movsd	-8(%rbp,%rbx,8), %xmm1
	subsd	%xmm4, %xmm1
	addsd	%xmm6, %xmm0
	movsd	8(%rbp,%rbx,8), %xmm6
	mulsd	%xmm9, %xmm1
	subsd	%xmm4, %xmm6
	mulsd	%xmm10, %xmm0
	movapd	%xmm6, %xmm4
	mulsd	%xmm7, %xmm4
	addsd	%xmm4, %xmm1
	mulsd	%xmm12, %xmm1
	addsd	%xmm1, %xmm0
	addsd	%xmm5, %xmm0
	mulsd	%xmm13, %xmm0
	addsd	%xmm3, %xmm0
	movsd	%xmm0, (%rax,%rbx,8)
	andpd	%xmm11, %xmm0
	comisd	%xmm2, %xmm0
	jbe	.L50
	movsd	%xmm0, (%r10)
	movapd	%xmm0, %xmm2
.L50:
	addq	$1, %rbx
	cmpl	%ebx, %r9d
	jg	.L53
	movq	200(%rsp), %rax
	movl	%r12d, %r11d
	movq	%rdi, %r12
	addq	$1, 8(%rsp)
	movq	136(%rsp), %rdi
	addq	%rax, 128(%rsp)
	addq	%rax, 184(%rsp)
	movq	8(%rsp), %rsi
	addq	%rax, %r13
	addq	%rax, %r12
	addq	%rax, 192(%rsp)
	addq	%rdi, 144(%rsp)
	addq	%rdi, 152(%rsp)
	cmpq	216(%rsp), %rsi
	jne	.L36
	movl	300(%rsp), %eax
	addq	$8, %r10
	cmpl	344(%rsp), %eax
	jne	.L39
.L30:
	leaq	384(%rsp), %rsi
	movl	$4, %edi
	call	clock_gettime@PLT
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1
	movq	392(%rsp), %rax
	subq	376(%rsp), %rax
	leaq	.LC16(%rip), %rsi
	movl	$1, %edi
	cvtsi2sdq	%rax, %xmm0
	movq	384(%rsp), %rax
	mulsd	.LC15(%rip), %xmm0
	subq	368(%rsp), %rax
	cvtsi2sdq	%rax, %xmm1
	movl	$1, %eax
	addsd	%xmm1, %xmm0
	call	__printf_chk@PLT
	leaq	.LC17(%rip), %rsi
	leaq	.LC18(%rip), %rdi
	call	fopen@PLT
	movl	Nx(%rip), %edx
	movl	$8, %esi
	imull	Ny(%rip), %edx
	movq	360(%rsp), %rdi
	movq	%rax, %rcx
	movslq	%edx, %rdx
	call	fwrite@PLT
	movl	Nt(%rip), %ecx
	testl	%ecx, %ecx
	jle	.L88
	leal	-1(%rcx), %eax
	cmpl	$1, %eax
	jbe	.L70
	movl	%ecx, %edx
	movq	352(%rsp), %rsi
	pxor	%xmm0, %xmm0
	shrl	%edx
	salq	$4, %rdx
	movq	%rsi, %rax
	addq	%rsi, %rdx
.L56:
	movupd	(%rax), %xmm2
	addq	$16, %rax
	maxpd	%xmm2, %xmm0
	cmpq	%rax, %rdx
	jne	.L56
	movapd	%xmm0, %xmm1
	movl	%ecx, %eax
	unpckhpd	%xmm0, %xmm1
	andl	$-2, %eax
	maxpd	%xmm1, %xmm0
	testb	$1, %cl
	je	.L33
.L54:
	movq	352(%rsp), %rsi
	movslq	%eax, %rdx
	addl	$1, %eax
	maxsd	(%rsi,%rdx,8), %xmm0
	cmpl	%eax, %ecx
	jle	.L33
	cltq
	maxsd	(%rsi,%rax,8), %xmm0
.L33:
	leaq	.LC20(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	movq	408(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L89
	addq	$424, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L62:
	.cfi_restore_state
	xorl	%eax, %eax
	jmp	.L16
.L87:
	movq	128(%rsp), %rax
	movq	176(%rsp), %rcx
	movl	$1, %ebx
	movq	208(%rsp), %rsi
	leaq	(%rcx,%rax), %rbp
	addq	%rsi, %rax
	jmp	.L38
.L63:
	xorl	%edx, %edx
	jmp	.L17
.L61:
	movsd	.LC4(%rip), %xmm0
	jmp	.L12
.L15:
	movl	$16, %edi
	call	malloc@PLT
	movq	%r15, %rdi
	movq	%rax, %rbx
	movq	%rax, 304(%rsp)
	call	malloc@PLT
	movq	%r15, %rdi
	movq	%rax, (%rbx)
	call	malloc@PLT
	movq	%rax, 360(%rsp)
	movq	%rax, 8(%rbx)
	jmp	.L58
.L88:
	pxor	%xmm0, %xmm0
	jmp	.L33
.L70:
	xorl	%eax, %eax
	pxor	%xmm0, %xmm0
	jmp	.L54
.L89:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE1820:
	.size	main, .-main
	.p2align 4
	.type	_GLOBAL__sub_I_Nx, @function
_GLOBAL__sub_I_Nx:
.LFB2301:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE2301:
	.size	_GLOBAL__sub_I_Nx, .-_GLOBAL__sub_I_Nx
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_Nx
	.globl	t
	.bss
	.align 8
	.type	t, @object
	.size	t, 8
t:
	.zero	8
	.globl	Sy
	.data
	.align 4
	.type	Sy, @object
	.size	Sy, 4
Sy:
	.long	1
	.globl	Sx
	.align 4
	.type	Sx, @object
	.size	Sx, 4
Sx:
	.long	1
	.globl	Nt
	.align 4
	.type	Nt, @object
	.size	Nt, 4
Nt:
	.long	100
	.globl	Ny
	.align 4
	.type	Ny, @object
	.size	Ny, 4
Ny:
	.long	8000
	.globl	Nx
	.align 4
	.type	Nx, @object
	.size	Nx, 4
Nx:
	.long	8000
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1073217536
	.align 8
.LC2:
	.long	1374389535
	.long	1075388088
	.align 8
.LC3:
	.long	0
	.long	-1078984704
	.align 8
.LC4:
	.long	1202590843
	.long	1065646817
	.align 8
.LC5:
	.long	3539053052
	.long	1062232653
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC6:
	.long	0
	.long	1
	.long	2
	.long	3
	.section	.rodata.cst8
	.align 8
.LC7:
	.long	0
	.long	1074790400
	.section	.rodata.cst16
	.align 16
.LC8:
	.long	4
	.long	4
	.long	4
	.long	4
	.align 16
.LC9:
	.long	1202590844
	.long	1065646817
	.long	1202590844
	.long	1065646817
	.align 16
.LC10:
	.long	1
	.long	1
	.long	1
	.long	1
	.section	.rodata.cst8
	.align 8
.LC11:
	.long	1202590844
	.long	1065646817
	.section	.rodata.cst16
	.align 16
.LC12:
	.long	1202590844
	.long	1067743969
	.long	1202590844
	.long	1067743969
	.section	.rodata.cst8
	.align 8
.LC13:
	.long	1202590844
	.long	1067743969
	.align 8
.LC14:
	.long	0
	.long	1071644672
	.align 8
.LC15:
	.long	3894859413
	.long	1041313291
	.section	.rodata.cst16
	.align 16
.LC19:
	.long	4294967295
	.long	2147483647
	.long	0
	.long	0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
