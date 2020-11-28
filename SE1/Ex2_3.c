#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

char *itoa(int value, char *str, int base){	//Exercício 2
	if(base<=0 || base>35 || (base!=10 && value<0)) return str;			//De forma a verificar se os valores a representar nao ultrapassam o Z, ou se o valor é negativo e a base diferente de 10
	
	int i;																//Indice onde escrever
	if(value<0){														//Caso para quando é negativo e a base 10, colocar o '-' antes
		value=-value;													//Inverter de forma a trabalhar com o valor positivo
		i=log10(value)/log10(base);										//Cálculo do último índice necessário, pois escrevemos da direita para a esquerda porque calculamos o Resto(%) pela base
		str[0]='-';														//Colocar o '-' no início
		i++;															//Incrementar o i pois colocámos o '-', logo temos mais 1 posição
	}else i=log10(value)/log10(base);									//Cálculo do último índice necessário, pois escrevemos da direita para a esquerda porque calculamos o Resto(%) pela base (No caso dos positivos)
	
	str[i+1]=0;															//Colocar o fim da String
	
	while(value>0){														//Ciclo para fazer a conversão
		str[i--]=(value%base)>9? (value%base)-10+'A':(value%base)+'0';	//Colocar o Resto do value pela base na posição i, e caso seja maior que 9 somar 'A'(Ex: se for 11%16=11 logo é 1+'A' = 'B')
		value/=base;													//Atualizar o value para calcularmos o próximo Resto
	}
	return str;															//Retorno do value convertido para a base especifícada no parâmetro
}

char aux[MAX_SIZE];

int main(int argc, char *argv[]){										//Exercício 3 - Teste da função itoa
	int base=10;														//Base 10 por omissão
	if(argc==2) base = atoi(argv[1]);									//Caso o utilizador introduza base
	
	int value;															//Valor a ser convertido
	
	while(fgets(aux, MAX_SIZE, stdin)!=NULL){							//Nota: Quando a ler valores do terminal queremos parar de ler, ao introduzir apenas o caracter '\n' ocorre "Segmentation fault (core dumped)" mas ao ler de um ficheiro/Simular ^D já termina sem erro, não consegui resolver o problema depois de várias tentativas
		value = atoi(aux);												//Conversão do valor lido como uma String para inteiro
		itoa(value, aux, base);											//Conversão do valor para a base pretendida
		printf("%d: %d: %s\n", value, (int)strlen(aux), aux);			//Print de acordo com o enunciado "nnn: len: vvv"
	}
}
