/* Função em C:

void memswap(void *one, void *other, size_t width) {
	char tmp[width];
	memcpy(tmp, one, width);
	memcpy(one, other, width);
	memcpy(other, tmp, width);
}*/	
	
	.text
	.globl	memswap
	.type	memswap, @function
memswap:					/* RDI = one | RSI = other | RDX = width */
	
	push 	%RBP 			/* Salvar RBP */
	push 	%R12			/* Salvar R12 */
	mov 	%RSP, %RBP		/* Salvar RSP */
	
	sub 	%RDX, %RSP		/* char tmp[width] */
	
	/* memcpy(tmp, one, width) */
	mov 	%RSI, %R12
	mov 	%RDI, %RSI
	mov 	%RSP, %RDI
	call 	memcpy
	
	/* memcpy(one, other, width) */
	mov 	%RSI, %RDI
	mov 	%R12, %RSI
	call 	memcpy
	
	/* memcpy(other, tmp, width) */
	mov		%RSI, %RDI
	mov		%RSP, %RSI
	call 	memcpy
	
	mov		%RBP, %RSP		/* Recolocar RSP */
	pop		%R12			/* Recolocar R12 */
	pop		%RBP			/* Recolocar RBP */
	ret
	
	.end
