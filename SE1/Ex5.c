#include <stdio.h>

typedef struct date{	//Exercício 5.A) Definição do tipo Date
	unsigned day:5;		//Campo: dia codificado com 5 bits (0-31) pois só existem no máximo 31 dias num mês
	unsigned month:4;	//Campo: mês codificado com 4 bits (0-15) pois só existem 12 meses
	unsigned year:23;	//Campo: ano codificado com os restantes 23 bits do Inteiro, de forma a que uma Date só ocupe 1 int
}Date;

typedef struct banking_node{	//Definição do tipo BankingNode
	Date data;
	char *desc;
	int type;
	struct banking_node *next;
}BankingNode;

int cmpDates(Date *d1, Date *d2){	//Função auxiliar para comparar duas variáveis do tipo Date
	return d1->year == d2->year? (d1->month==d2->month? d1->day - d2->day: d1->month - d2->month) : d1->year - d2->year; //Retorna a diferença entre a primeira Date e a segunda
}

void get_banking_by_category(BankingNode *categories[], BankingNode movements[], size_t size){	//Exercício 5.B)
	for(int i=0; i<size; i++){											//Ciclo que percorre o Array movements de forma a inserir cada movement no Array categories
		BankingNode *aux = &movements[i];								//Ponteiro auxiliar para o movimento a inserir
		if(categories[aux->type]==NULL){ 								//Caso para quando a categoria ainda não tem movimentos
			categories[aux->type]=aux;									//Colocar o movimento na primeira posição da categoria em questão
		}else if(cmpDates(&aux->data, &categories[aux->type]->data)<0){ //Caso para quando o movimento é mais antigo do que o movimento da cabeça da lista
			aux->next = categories[aux->type];							//Afectar o campo next do movimento a inserir com o ponteiro para a cabeça da lista
			categories[aux->type]=aux;									//Colocar o movimento a inserir como o primeiro do tipo em questão
		}else{															//Caso para quando temos que inserir o movimento no meio ou no final da lista de um certo tipo
			BankingNode *p = categories[aux->type];						//Ponteiro auxiliar para iterar a lista de um certo tipo de movimentos
			for(; p->next!=NULL; p=p->next){							//Ciclo para procurar a posição onde temos que inserir o movimento
				if(cmpDates(&aux->data, &p->data)<0){					//Caso para quando o movimento a inserir é mais antigo que o movimento do ponteiro auxiliar
					aux->next = p;										//Afectar o campo next do movimento a inserir com o ponteiro para o próximo que vinha na lista
					p=aux;												//Afectar o campo next do anterior com o ponteiro para o movimento a inserir
					break;												//Parar pois já inserimos o movimento
				}
			}
			p->next=aux;												//Caso o movimento seja o mais recente, é colocado na última posição da lista, afectando o campo next do último movimento com o ponteiro para o movimento a inserir
		}
	}
}

BankingNode *categories[3];	//Array de ponteiros para BankingNodes onde cada índice do array indica a categoria, 3 categorias devido ao exercício 5.C)

int main(){	//Exercício 5.C) Programa de Teste das funções definidas
	BankingNode movements[]={					//Array de 9 Movimentos
		{{3,2,2015}, "movimento1", 0, NULL},
		{{4,2,2015}, "movimento2", 2, NULL},
		{{4,2,2014}, "movimento3", 1, NULL},
		{{23,6,2011}, "movimento4", 2, NULL},
		{{9,11,2009}, "movimento5", 1, NULL},
		{{4,5,2015}, "movimento6", 0, NULL},
		{{2,2,2015}, "movimento7", 0, NULL},
		{{2,9,2015}, "movimento8", 2, NULL},
		{{30,7,2017}, "movimento9", 1, NULL}
		};
	get_banking_by_category(categories, movements, 9);	//Chamada à função de organização
	
	//Print do Array categories de forma a verificar se organizou com sucesso os movimentos
	for(int i=0; i<3; i++){
		BankingNode *p=categories[i];
		printf("Categoria %d: ", i);
		while(p!=NULL){
			printf("%s ", p->desc);
			p=p->next;
		}
		printf("\n");
	}
	//-------------------------------------------------------------------------------------
}
