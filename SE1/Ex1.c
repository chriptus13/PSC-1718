#include <stdio.h>

int int_change(int value, int nbits, int pattern_to_change, int new_pattern){	//Exercício 1
	int mask=(1<<nbits)-1; 								//Máscara com o nº de bits a substituir
	pattern_to_change&=mask; 							//Verificar que o padrão a mudar tem nbits
	new_pattern&=mask;									//Verificar que o novo padrão tem nbits
	while((value<<1) > mask){							//Ciclo para procurar o padrão a mudar no value
		if((mask & value)==pattern_to_change) break;	//Se o valor da posicao atual for igual ao padrao a mudar ja podemos terminar o ciclo e retornar o novo valor
		new_pattern<<=1;								//Shift do novo padrão para quando encontrarmos o padrão este já estar no sítio onde irá ser colocado
		pattern_to_change<<=1;							//Shift do padrão a mudar para podermos verificar nas diferentes posições
		mask<<=1;										//Shift da máscara para "incrementar" a posição a procurar no value
	}
	return (value & (~mask)) | new_pattern;				//Retorno do novo valor com o novo padrão no sítio onde encontrámos o pattern_to_change
}

int main(){	//Teste da função int_change
	printf("%x\n", int_change(0x2AD555BC, 5, 0x15, 0x2A));
}
