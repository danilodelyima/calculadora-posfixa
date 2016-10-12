#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"

#ifdef __linux__

#define CLEAR "clear"

#elif _WIN32

#define CLEAR "cls"

#endif

void formataEntrada(char expressao[]){
    char result[50];
    int i = 0;
    int aux = 1;
    getchar();
    printf("Digite a expressao: \n");
    scanf("%[^\n]s", expressao);
    result[0] = ' ';
    while(expressao[i] != '\0'){

        if((expressao[i] == '+' || expressao[i] == '-' || expressao[i] == '*' ||
           expressao[i] == '/')  && expressao[i-1] != ' '){

            result[aux++] = expressao[i];
            result[aux++] = ' ';
        }else
            result[aux++] = expressao[i];
        i++;
    }

    result[aux] = '\0';
    strcpy(expressao, result);
}

int validaExpressao(char expressao[], t_pilha *pilha){
    int valida = 1;
    int i = 0;
    char c;

    while( (expressao[i] != '\0') && (valida != 0) ){
        if((expressao[i] != 32 && expressao[i] < 40) || expressao[i] == 44 || expressao[i] == 46 ||
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
            if (expressao[i+1] >= 48 || expressao[i+1] <= 57 ){
                posfixa[cont++] = expressao[i];
            }else{
                posfixa[cont++] = expressao[i];
                posfixa[cont++] = ' ';
            }

        }
        i++;
    }
    while(!pilhaVazia(pilha))
       posfixa[cont++] =  pop(pilha);

    posfixa[cont] = '\0';
}

void empilhaOperando(char split[], char posfixa[], t_pilha *pilha, int inicio, int fim){
    int aux = 0, j;

    for(j=inicio; j<fim; j++)
        split[aux++] = posfixa[j];
    split[aux] = '\0';
    push(pilha, atoi(split));
    aux = 0;
}

void calculaExpressao(char posfixa[], t_pilha *pilha){
    char split[10];
    int i = 0;
    int marcador_inicio = 0, marcador_fim = 0;
    int op1, op2;

    while(posfixa[i] != '\0'){
        marcador_fim = i;

        if(posfixa[i] == ' '){
            if(marcador_fim - marcador_inicio > 1)
                empilhaOperando(split, posfixa, pilha, marcador_inicio, marcador_fim);
            i++;
            marcador_inicio = marcador_fim;
            continue;

        }else if(posfixa[i] == '+' || posfixa[i] == '-' ||
                 posfixa[i] == '*' || posfixa[i] == '/' ){

            if(marcador_fim - marcador_inicio > 1 )
                empilhaOperando(split, posfixa, pilha, marcador_inicio, marcador_fim);

            marcador_inicio = marcador_fim;

            switch(posfixa[i]){
                case '+':
                    op2 = pop(pilha);
                    op1 = pop(pilha);
                    push(pilha, op1+op2);
                    break;
                case '-':
                    op2 = pop(pilha);
                    op1 = pop(pilha);
                    push(pilha, op1-op2);
                    break;
                case '*':
                    op2 = pop(pilha);
                    op1 = pop(pilha);
                    push(pilha, op1*op2);
                    break;
                case '/':
                    op2 = pop(pilha);
                    op1 = pop(pilha);
                    push(pilha, op1/op2);
                    break;
            }
        }
        i++;
    }
}

void resolucaoExpressao(char expressao[], t_pilha *pilha){
    char posfixa[100];

    if (validaExpressao(expressao, pilha)){
        system(CLEAR);
        printf("Expressao valida\n");
        posFixa(expressao, posfixa, pilha);
        printf("Expressao posfixa:\n%s\n", posfixa);
        calculaExpressao(posfixa, pilha);
        printf("Resultado:\n");
        imprimirPilha(pilha);
        printf("Pressione ENTER para continuar\n");
        getchar();
        getchar();
    }
    else{
        printf("Expressao invalida");
    }
}

int verificaOperandos(t_pilha *pilha){

    if(pilha->topo < 1){
        printf("NUMERO DE OPERANDOS INSUFICIENTE\n\n");
        printf("Pressione ENTER para continuar\n");
        getchar();
        getchar();
        return 0;
    }else
        return 1;
}

void operacaoBasica(t_pilha *pilha, char entrada[]){
    int op1, op2;

    if(verificaOperandos(pilha)){
        op1 = pop(pilha);
        op2 = pop(pilha);

        if (strcmp(entrada, "+") == 0)
            push(pilha, op1+op2);
        else if (strcmp(entrada, "-") == 0)
            push(pilha, op1-op2);
        else if (strcmp(entrada, "*") == 0)
            push(pilha, op1*op2);
        else
            push(pilha, op1/op2);
    }
}

void operacaoRepeticao(t_pilha *pilha, char entrada[]){
    int result = 0, i;

    if(verificaOperandos(pilha)){

        if (strcmp(entrada, "+!") == 0)
            for(i = pilha->topo; i > VAZIA; i--)
                result += pop(pilha);
        else if (strcmp(entrada, "-!") == 0){
            result = pop(pilha);
            for(i = pilha->topo; i > VAZIA; i--)
                result -= pop(pilha);
        }
        else if(strcmp(entrada, "*!") == 0){
            result = pop(pilha);
            for(i = pilha->topo; i > VAZIA; i--)
                result *= pop(pilha);
        }
        push(pilha, result);
    }
}

void copiaDeElemento(t_pilha *pilha, char entrada[]){
    int i, repeticoes, elemento;

    if(verificaOperandos(pilha)){

        repeticoes = pop(pilha);
        elemento = pop(pilha);

        for(i=1; i<= repeticoes;i++)
            push(pilha, elemento);
    }
}

void calculadora(t_pilha *pilha){
    char entrada[10];
    int valor;
    system(CLEAR);
    printf("Modo calculadora\n");
    printf("Digite 'sair' para sair do modo calculadora\n");

    do{
        imprimirPilha(pilha);
        printf("->");
        scanf("%s", entrada);

        if(strcmp(entrada, "+") == 0 || strcmp(entrada, "-") == 0 ||
           strcmp(entrada, "*") == 0 || strcmp(entrada, "/") == 0){

            operacaoBasica(pilha, entrada);

        }else if(strcmp(entrada, "+!") == 0 || strcmp(entrada, "-!") == 0 ||
                 strcmp(entrada, "*!") == 0){

            operacaoRepeticao(pilha, entrada);

        }else if(strcmp(entrada, "c") == 0){

            copiaDeElemento(pilha, entrada);

        }else{
            valor = atoi(entrada);
            push(pilha, valor);
        }
        system(CLEAR);
    }while(strcmp (entrada, "sair") != 0);
}

int main(){
    char expressao[50];
    int opcao;
    t_pilha* pilha = getPilha(30);
    t_pilha* calc = getPilha(30);

    printf("1.Resolucao de expressao\n");
	printf("2.Calculadora\n");
	printf("3.SAIR\n");
	printf("ESCOLHA: ");
	scanf("%d", &opcao);

	switch(opcao){
		case 1:
		    formataEntrada(expressao);
			resolucaoExpressao(expressao, pilha);
			system(CLEAR);
			main();
		break;

		case 2:
			calculadora(calc);
			system(CLEAR);
			main();
		break;

		case 3:
		    system(CLEAR);
		    printf("Saindo...\n");
		    return 0;
        break;

		default:
			system(CLEAR);
			printf("VALOR INVALIDO!\nDigite um valor de 1 - 3\n");
			main();
    }

    liberaPilha(pilha);
    liberaPilha(calc);

    return 0;
}
