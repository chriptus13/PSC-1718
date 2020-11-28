/* Função em C:

void quick_sort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *)) {
	if (nel < 2) return;
	
	char *limit, *last, *ptr;
	
	limit = (char *)base + (nel * width);
	
	memswap(base, (char *)base + (nel >> 1) * width, width);
	
	for (last = base, ptr = (char *)base + width; ptr < limit; ptr += width)
		if ((*compar)(ptr, base) < 0) memswap(ptr, last += width, width);
		
	memswap(base, last, width);
	quick_sort(base, (last - (char *)base) / width, width, compar);
	quick_sort(last + width, ((limit - last) / width) - 1, width, compar);
}*/	
	
	.text
	.globl	quick_sort
	.type	quick_sort, @function
quick_sort: /* RDI = base | RSI = nel | RDX = width | RCX = compar */
	
	/* if(nel<2) return; */
	cmp 	$2, %RSI
	jge 	qs_cont
	ret
	
qs_cont:
	push	%R12	/* limit */
	push	%R13	/* base */
	push	%R14	/* width */
	push	%R15	/* compar */
	push	%RBP	/* ptr */
	push	%RBX	/* last */
	
	mov		%RCX, %R15
	mov		%RDX, %R14
	mov		%RDI, %R13
	
	/* limit = (char *)base + (nel*width); */
	mov		%RSI, %RAX
	mul 	%R14
	mov		%RAX, %R12
	add		%R13, %R12
	
	/* memswap(base, (char *)base + (nel>>1)*width, width); */
	shr		%RSI
	mov		%RSI, %RAX
	mul		%R14
	mov		%RAX, %RSI
	add		%R13, %RSI
	mov		%R14, %RDX
	call 	memswap
	
	/* last = base */
	mov		%R13, %RBX
	
	/* ptr = (char *)base + width; */
	mov		%R13, %RBP
	add		%R14, %RBP
	
qs_for:
	/* ptr < limit */ 
	cmp		%R12, %RBP
	jge		qs_forend
	
	/* if((*compar)(ptr, base) < 0) */
	mov		%RBP, %RDI
	mov		%R13, %RSI
	call 	*%R15
	cmp 	$0, %EAX
	jge		qs_forcont
	
	/* memswap(ptr, last+=width, width); */
	mov		%RBP, %RDI
	add		%R14, %RBX
	mov		%RBX, %RSI
	mov		%R14, %RDX
	call 	memswap
	
qs_forcont:
	/* ptr+=width */
	add		%R14, %RBP
	jmp		qs_for
	
qs_forend:
	/* memswap(base, last, width); */
	mov		%R13, %RDI
	mov		%RBX, %RSI
	mov		%R14, %RDX
	call 	memswap
	
	/* quick_sort(base, (last - (char *)base)/width, width, compar); */
	mov 	%R13, %RDI
	mov		%RBX, %RAX
	sub		%RDI, %RAX
	xor		%RDX, %RDX
	div		%R14
	mov		%RAX, %RSI
	mov		%R14, %RDX
	mov		%R15, %RCX
	call 	quick_sort
	
	/* quick_sort(last+width, ((limit-last)/width)-1, width, compar); */
	mov		%RBX, %RDI
	add		%R14, %RDI
	mov		%R12, %RAX
	sub		%RBX, %RAX
	xor		%RDX, %RDX
	div		%R14
	dec		%RAX
	mov		%RAX, %RSI
	mov		%R14, %RDX
	mov		%R15, %RCX
	call 	quick_sort
	
	pop		%RBX
	pop		%RBP
	pop		%R15
	pop		%R14
	pop		%R13
	pop		%R12
	ret

	.end
