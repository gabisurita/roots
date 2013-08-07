/* 
** O programa a seguir usa uma aproximação numérica do TVM para encontrar as raizes de uma equação
** 
*/
 
#include <stdlib.h> 
#include <stdio.h>
//#include <math.h> //Use para gerar funções mais interessantes

/*
** O PROGRAMA POSSUÍ 4 PARTES!
*/

//1. Define intervalo de busca e numero de amostragens
#define samples 1000
double Start =  100;
double End   = -100;

//2. Define a função desejada
void DefineFunc(double Fx[2][samples], double min, double max);

//3. Estruturas de armazenamento de raizes
typedef struct node Node;
Node* Insert(Node* last, double min, double max);
Node* Pop(Node* point, double x[2]);

//4. Funções do método numérico
int ExistRoot(double a, double b);
Node* FindRoot(double a,  double b);



/*
** DEFINIÇÂO DA FUNÇÂO
*/

// Define numericamente a funcao de x num dado intervalo usando a amostragem definida
void DefineFunc(double Fx[2][samples], double min, double max){
  double interval = (max - min)/samples;

	for (int i = 0; i <samples; i++){
		double x = min + i*interval;
		Fx[0][i] = x;
		Fx[1][i] = x*x - 3.5*x + 3; // SUBSTITUA PELA FUNÇÃO DESEJADA
	}
	return;
}


/*
** INICIO DO MÉTODO NUMÉRICO
*/


// Define se existe raiz no intervalo [a,b]
int ExistRoot(double a, double b){
	if (a*b > 0)
		return 0;  
	if (a*b < 0)
		return 1;
	else
		return 255; //What a luck!!
}
	

// Encontra o intervalo que contem as raizes no intervalo [a,b] com precisao (b-a)/amostragem 
Node* FindRoot(double a,  double b){
	Node* Results = NULL;

	double Fx[2][samples];
	DefineFunc(Fx, a, b);

	double last[2];
	last[1] = Fx[1][0];

	for (int i = 1; i <samples; i++){
		int result = ExistRoot(last[1], Fx[1][i]);
		if (result){
			if (result > 1){
				Results = Insert(Results, Fx[0][i], Fx[0][i]);
				i++;
			}
			else{
				Results = Insert(Results, last[0], Fx[0][i]);
			}
		}
		last[0] = Fx[0][i];
		last[1] = Fx[1][i];
			
	}
	return Results;
}

// Roda o ciclo de busca de raizes 2x, uma para busca e outra para refino.
int main()
{
	Node* RefinedResults = NULL;
	Node* Results = FindRoot(Start, End);

	if (Results != NULL){
		while (Results  != NULL){
			double x[2];
			Results = Pop(Results, x);
			printf("1st Cycle: %lf %lf", x[0],x[1]);
			printf("\n");						
			if (x[0] != x[1]){			
				RefinedResults = FindRoot(x[0], x[1]);
				while (RefinedResults  != NULL){
					RefinedResults = Pop(RefinedResults, x);
					printf("2nd Cycle: %lf %lf", x[0],x[1]);
					printf("\n");
				}
			}
			printf("\n");
		}
	}
	return 0;
}

/*
** FIM DO MÉTODO NUMÉRICO
*/


/*
** AS ESTRUTURAS A SEGUIR SERVEM APENAS PARA ARMAZENAR DINAMICAMENTE AS RAIZES ENCONTRADAS 
*/

// Lista ligada para armazenamento
struct node {
	struct node* next;
	double min;
	double max;
};

// Funcao de insercao no inicio da lista
Node* Insert(Node* last, double min, double max){
	Node* temp = (Node*) malloc(sizeof(Node));
	temp->next = last;
	temp->min = min;
	temp->max = max;
	return temp;
}	

// "Retira" o ultimo intervalo a ser inserido na lista
Node* Pop(Node* point, double x[2]){
	x[0] = point->min;
	x[1] = point->max;
	Node* temp = point->next;
	free(point);
	return temp;
}

/*
** FIM DAS ESTRUTURAS DE ARMAZENAMENTO
*/
