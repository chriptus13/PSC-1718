#include <stdio.h>

typedef union float_{			//Estrutura de forma a podermos analisar os bits do float. Usamos uma union do float com um inteiro dividido nas respectivas partes do float
	float f;					//Valor do float
	struct{						//Struct para podermos aceder às respectivas partes de um float
		unsigned mantissa:23;	//Mantissa do float
		unsigned exp:8;			//Expoente do float
		unsigned signal:1;		//Sinal do float
	};
}Float;

int round_to_int(float value, int *ivalue){
	Float f;	//Crição da union auxiliar para analise dos bits do float
	f.f=value;	//Afetação do campo f da union com o value
	
	if(f.exp-127<=0){															//Caso para quando o expoente é menor ou igual a 127
		if(f.exp==127) *ivalue = (f.mantissa & 0x400000)? 2: 1;					//Quando o expoente é 127: (-1)^signal * 2^(0) * 1,mantissa = (-1)^signal * (2 caso o primeiro bit da Mantissa seja 1 ou 1 caso esse bit seja 0)
		else if(f.exp==126) *ivalue = 1;										//Quando o expoente é 126: (-1)^signal * 2^(-1) * 1,mantissa = 1 pois temos sempre o bit que vale 0.5 a 1, devido ao shift para a direita (1,mantissa)>>1 = 0,5+mantissa
		else *ivalue = 0;														//Quando o expoente é menor que 126 dá sempre 0 pois nunca iremos ter o bit de que vale 0,5 a 1
	}else if(f.exp-127==128) return -1;											//Quando o expoente é 255 dá erro pois ou dá infinito/-infinito ou NaN logo retorna -1(erro)
	else{																		//Caso para quando o expoente é maior que 127
		if(f.mantissa==0) *ivalue = (1<<(f.exp-127));							//Quando a mantissa é 0: (-1)^signal * 2^(exp-127) * 1,0
		else{																	//Caso para quando o expoente é maior que 127 e a mantissa não é 0
			int aux = 0x800000 | f.mantissa;									//Acrescentar 1 bit à esquerda da mantissa, de forma a ser 1,mantissa
			int xp = f.exp-127;													//Calcular o expoente-127
			if(xp<23){															//Caso para quando o expoente-127 é menor que 23
				*ivalue = ((aux>>(23-xp-1))&1)? (aux>>(23-xp))+1 : aux>>(23-xp);//Quando o expoente-127 é menor que 23 o valor é simplesmente 1,mantissa shiftado xp posições para direita, +1 caso o último shift der carry
			}else *ivalue = aux<<(xp-23);										//Quando o expoente-127 é maior ou igual a 23 basta shiftar 1,mantissa xp-23 posições para a esquerda.
		}
	}
	*ivalue *= (f.signal? -1: 1);	//Colocar o valor final a negativo caso o value seja negativo
	return 0;	//Retorno de 0 quando value é convertido correctamente
}

int main(){	//Teste da função round_to_int
	float f = 2.5;
	int a;
	printf("%d\n", round_to_int(f, &a));	//Print do retorno da função, de forma a ver se deu erro ou converteu
	printf("%d\n", a);	//Print do valor convertido
}
