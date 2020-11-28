/* Função em C:

void *memset(void *ptr, int value, size_t len){
	long aux = value&255;
	aux |= aux<<8;
	aux |= aux<<16;
	aux |= aux<<32;
	
	while(len){
		if(len>=8 && !(((unsigned long) ptr)&15)){
			*((long*) ptr) = aux;
			ptr+=8;
			len-=8;
		}else{
			*((char*) ptr++) = (char)aux;
			len--;
		}
	}
}*/

	.text
	.globl	memset
	.type	memset, @function
memset:							/* RDI = *ptr | RSI = value | RDX = len */
	pushq %RCX					/* Salvar RCX */
	
	andq $255, %RSI				/* long aux = value&255 */
	
	/* aux |= aux<<8 */
	movq %RSI, %R10
	mov $8, %CL
	shl %CL, %R10
	or %R10, %RSI
	/****************/
	
	/* aux |= aux<<16 */
	mov $16, %CL
	movq %RSI, %R10
	shl %Cl, %R10
	or %R10, %RSI
	/******************/
	
	/* aux |= aux<<32 */
	mov $32, %CL
	movq %RSI, %R10
	shl %Cl, %R10
	or %R10, %RSI
	/******************/
	
memset_while:	
	/* while(len) */
	cmp $0, %RDX
	jz memset_end
	/*************/
	
	/* if(len>=8 && !(((unsigned long) ptr)&15)) */
	cmp $8, %RDX
	jl memset_else
	test $15, %RDI
	jnz memset_else
	/*********************************************/
	
	movq %RSI, (%RDI)			/* *((long*) ptr) = aux */
	add $8, %RDI				/* ptr+=8 */
	sub $8, %RDX				/* len-=8 */
	jmp memset_while
	
memset_else:					/* else */
	mov %RSI, (%RDI)			/* *((char*) ptr) = (char)aux */
	inc %RDI					/* ptr++ */
	dec %RDX					/* len-- */
	jmp memset_while
	
memset_end:
	popq %RCX					/* Recolocar RCX */
	ret
	.end
