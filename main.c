#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"

#ifdef __linux__

#define CLEAR "clear"

#elif _WIN32

#define CLEAR "cls"

#endif

int validaExpressao(char expressao[], t_pilha *pilha){
    int valida = 1;
    int i = 0;
    char c;
    printf("Digite a expressão: \n");
    scanf("%[^\n]", expressao);

    while( (expressao[i] != '\0') && (valida != 0) ){
        if(expressao[i] < 40 || expressao[i] == 44 || expressao[i] == 46 ||
          (expressao[i] > 57 && expressao[i] < 91) || expressao[i] == 92 ||
          (expressao[i] > 93 && expressao[i] < 123) || expressao[i] == 124 ||
           expressao[i] > 125){
            valida = 0;
            continue;
        }

        if((expressao[i] == '(') || (expressao[i] == '[') || (expressao[i] == '{'))
            push(pilha, (int) expressao[i]);

        if((expressao[i] == ')') || (expressao[i] == ']') || (expressao[i] == '}')){
            if( pilhaVazia(pilha) )
                valida = 0;
            else{
                c = (char) pop(pilha);

                switch(expressao[i]){
                    case ')':
                        if( c != '(' )
                            valida = 0;
                        break;
                    case ']':
                        if( c != '[' )
                            valida = 0;
                        break;
                    case '}':
                        if( c != '{' )
                            valida = 0;
                        break;
                }
            }
        }
        i++;
    }

    if(pilhaVazia(pilha) && valida == 1)
        return 1;
    else
        return 0;
}

int prioridade(char operador){
    if(operador == '+' || operador == '-')
        return 2;
    else if(operador == '*' || operador == '/')
        return 3;
    else
        return 1;

}

void posFixa(char expressao[], char posfixa[], t_pilha *pilha){
    int i = 0, cont = 0;
    while( expressao[i] != '\0' ){

        if(expressao[i] == '+' || expressao[i] == '-' ||
           expressao[i] == '*' || expressao[i] == '/' ){

            if(!pilhaVazia(pilha) &&
              (prioridade((char) topo(pilha)) >= prioridade(expressao[i]))){

                posfixa[cont++] = pop(pilha);
                push(pilha, (int) expressao[i]);

            }else{
                push(pilha, (int) expressao[i]);
            }
        }else if(expressao[i] == '(' || expressao[i] == '[' ||
                 expressao[i] == '{'){
            push(pilha, (int) expressao[i]);
        }
        else if(expressao[i] == ')' || expressao[i] == ']' ||
                expressao[i] == '}'){

            switch(expressao[i]){
                case ')':
                    while((char) topo(pilha) != '(' )
                        posfixa[cont++] = pop(pilha);

                    pop(pilha);
                    break;
                case ']':
                    while((char)topo(pilha) != '[' )
                        posfixa[cont++] = pop(pilha);

                    pop(pilha);
                    break;
                case '}':
                    while((char)topo(pilha) != '{' )
                       posfixa[cont++] = pop(pilha);

                    pop(pilha);
                    break;
            }
        }else{
            posfixa[cont++] = expressao[i];
        }
        i++;
    }
    while(!pilhaVazia(pilha))
       posfixa[cont++] =  pop(pilha);

    posfixa[cont] = '\0';
}

void resolucaoExpressao(char expressao[], t_pilha *pilha){

    if (validaExpressao(expressao, pilha)){
        printf("Expressao valida\n");
    }
    else{
        printf("Expressao invalida");
    }
}

void calculadora(t_pilha *pilha){
    char entrada[10];
    int valor, op1, op2;
    system(CLEAR);
    printf("Modo calculadora\n");
    printf("Digite 'sair' para sair do modo calculadora\n");

    do{
        imprimirPilha(pilha);
        printf("->");
        scanf("%s", entrada);
        if(strcmp(entrada, "+") == 0 || strcmp(entrada, "-") == 0 ||
           strcmp(entrada, "*") == 0 || strcmp(entrada, "/") == 0){
            if(pilha->topo < 1){
                printf("NUMERO DE OPERANDOS INSUFICIENTE\n\n");
            }else{
                op1 = pop(pilha);
                op2 = pop(pilha);

                if (strcmp(entrada, "+") == 0)
                        push(pilha, op1+op2);
                else if (strcmp(entrada, "-") == 0)
                        push(pilha, op1-op2);
                else if (strcmp(entrada, "*") == 0)
                        push(pilha, op1*op2);
                else if (strcmp(entrada, "/") == 0)
                        push(pilha, op1/op2);
            }
        }else{
            valor = atoi(entrada);
            push(pilha, valor);
        }
        system(CLEAR);
    }while(strcmp (entrada, "sair") != 0);

}
int main(){
    char expressao[100];
    int opcao;
    t_pilha* pilha = getPilha(10);
    t_pilha* calc = getPilha(20);

    printf("1.Resolucao de expressao\n");
	printf("2.Calculadora\n");
	printf("3.SAIR\n");
	printf("ESCOLHA: ");
	scanf("%d", &opcao);

	switch(opcao){
		case 1:
			resolucaoExpressao(expressao, pilha);
			main();
		break;

		case 2:
			calculadora(calc);
			system(CLEAR);
			main();
		break;

		case 3:
		    printf("Saindo...\n");
		    return 0;
        break;

		default:
			system(CLEAR);
			printf("VALOR INVALIDO!\nDigite um valor de 1 - 3\n");
			main();
    }

    /*
    if(!validaExpressao(expressao, pilha))
        printf("invalido\n");
    else{
        posFixa(expressao, posfixa, pilha);
        printf("%s\n", posfixa);
    }
    */

    liberaPilha(pilha);

    return 0;
}
