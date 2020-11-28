/* Função em C:

long get_total_work_time_after(Worker *worker, long start_time){
	long totalTime=0;
	CheckPoint *aux = worker->check_points;
	for(int i=0; i<worker->check_points_len; i++){
		if(aux[i].entry_time >= start_time) totalTime+= aux[i].exit_time - aux[i].entry_time;
		else if(aux[i].exit_time > start_time) totalTime+= aux[i].exit_time - start_time;
	}
	return totalTime;
}*/
	
	.text
	.globl	get_total_work_time_after
	.type	get_total_work_time_after, @function
get_total_work_time_after:		/* RDI = Ponteiro para o Worker | RSI = start_time */

	pushq %R12					/* Salvar R12 */
	pushq %R13					/* Salvar R13 */

	movq $0, %RAX				/* long totalTime=0 */
	
	leaq 24(%RDI), %R11			/* CheckPoint *aux = worker->check_points */
	
	movq 16(%RDI), %RDI			/* RDI = worker->check_points_len */

	movq $0, %R10				/* int i=0 */
	
gtwta_for:	

	/* i<worker->check_points_len */
	cmp %RDI, %R10
	jge gtwta_end
	/*****************************/
	
	/* if(aux[i].entry_time >= start_time) */
	leaq (%R11, %R10, 8), %R13
	movq (%R13, %R10, 8), %R13
	
	cmp %RSI, %R13
	jl gtwta_else 
	/**************************************/
	
	/* totalTime+= aux[i].exit_time - aux[i].entry_time */
	leaq (%R11, %R10, 8), %R12
	movq 8(%R12, %R10, 8), %R12
	
	sub %R13, %R12
	add %R12, %RAX
	jmp gtwta_cont
	/***************************************************/
		
gtwta_else:	

	/* else if(aux[i].exit_time > start_time) */
	leaq (%R11, %R10, 8), %R12
	movq 8(%R12, %R10, 8), %R12
	
	cmp %RSI, %R12
	jle gtwta_cont
	/*****************************************/
	
	/* totalTime+= aux[i].exit_time - start_time */
	sub %RSI, %R12
	add %R12, %RAX
	/********************************************/
	
gtwta_cont:
	/* i++ */
	inc %R10
	jmp gtwta_for
	/*******/
	
gtwta_end:
	popq %R13				/* Recolocar R13 */
	popq %R12				/* Recolocar R12 */
	ret
	.end
