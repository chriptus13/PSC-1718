/* Função em C:

void rotate_left(unsigned long value[], size_t n){
	n%=128;
	if(n==0) return;
	long aux;
	if(n>=64){
		aux=value[1];
		value[1]=value[0];
		value[0]=aux;
		if(n!=64) rotate_left(value, n-64);
		return;
	}
	aux = value[0];
	value[0]=value[1]>>(64-n) | value[0]<<n;
	value[1]=aux>>(64-n) | value[1]<<n;
	
}*/

	.text
	.globl	rotate_left
	.type	rotate_left, @function
rotate_left:		/* RDI = unsigned long value[] | RSI = n */

	push %RCX		/* Salvar RCX */
	push %R12		/* Salvar R12 */
	
	and $127, %RSI	/* n%=128 */
	cmp $0, %RSI	/* if(n==0) */
	jz rl_end
	
	cmp $64, %RSI	/* if(n>=64) */
	jc rl_else_1
	
	movq 8(%RDI), %RCX	/* aux=value[1] */
	
	/* value[1]=value[0] */
	movq (%RDI), %R10
	movq %R10, 8(%RDI)
	/********************/
	
	movq %RCX, (%RDI)	/* value[0]=aux */
	
	cmp $64, %RSI	/* if(n!=64) */
	jz rl_end
	
	/* rotate_left(value, n-64) */
	sub $64, %RSI	
	call rotate_left
	/***************************/
	
	jmp rl_end
	
rl_else_1:
	
	movq (%RDI), %R10	/* aux = value[0] */
	
	/* value[0]=value[1]>>(64-n) | value[0]<<n */
	movq 8(%RDI), %R11		
	movq %RSI, %R12
	movq $64, %RCX
	sub %R12, %RCX
	shr %CL, %R11
	
	movq %RSI, %RCX
	movq %R10, %R12
	sal %CL, %R12
	
	or %R12, %R11
	movq %R11, (%RDI)
	/******************************************/
	
	/* value[1]=aux>>(64-n) | value[1]<<n */
	movq 8(%RDI), %R11
	sal %CL, %R11
	movq $64, %R12
	sub %RCX, %R12
	movq %R12, %RCX
	shr %CL, %R10
	or %R10, %R11
	movq %R11, 8(%RDI)
	/******************************************/
	
rl_end:
	pop %R12		/* Recolocar o R12 */
	pop %RCX		/* Recolocar o RCX */
	ret
	.end
