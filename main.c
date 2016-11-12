/*
Estrutura de Dados, turma B
Arquivo: main.c
Nome: Danilo de Lima Cabrinha
Matrícula: 160026270
Descrição: A primeira opção transforma expressões infixas em pós-fixas e calcula
a expressão pós-fixada com auxílio de pilha.
A segunda opção implementa uma calculadora em forma de pilha.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"

#ifdef __linux__

#define CLEAR "clear"

#elif _WIN32

#define CLEAR "cls"

#endif
/*! Valida Entrada. */
/*!
Valida a entrada de expressoes, permitindo somente operadores e operandos e
adiciona espaços na string para serem usados na resoluçao
da expressao posfixa.
*/
int validaEntrada(char expressao[]){
    char result[50];
    int i = 0;
    int aux = 1;
    getchar();
    printf("Digite a expressao: \n");
    scanf("%[^\n]s", expressao);
    result[0] = ' ';

    while(expressao[i] != '\0'){

        if(expressao[i] < 40 || expressao[i] == 44 || expressao[i] == 46 ||
          (expressao[i] > 57 && expressao[i] < 91) || expressao[i] == 92 ||
          (expressao[i] > 93 && expressao[i] < 123) || expressao[i] == 124 ||
           expressao[i] > 125){
               return 0;
           }

        if(expressao[i] == '+' || expressao[i] == '-' || expressao[i] == '*' ||
           expressao[i] == '/') {

            result[aux++] = expressao[i];
            result[aux++] = ' ';
        }else
            result[aux++] = expressao[i];
        i++;
    }

    result[aux] = '\0';
    strcpy(expressao, result);

    return 1;
}


/*! Valida Expressao. */
/*!
Valida a expressao verificando se ha operadores de escopo abertos e fechados
em quantidades iguais.
*/

int validaExpressao(char expressao[], t_pilha *pilha){
    int valida = 1;
    int i = 0;
    char c;

    while( (expressao[i] != '\0') && (valida != 0) ){

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

/*! prioridade. */
/*!
Define a prioridade entre os operadores dando menor valor para o mais importante.
*/
int prioridade(char operador){
    if(operador == '+' || operador == '-')
        return 2;
    else if(operador == '*' || operador == '/')
        return 3;
    else
        return 1;
}

/*! posFixa. */
/*!
Transforma a expressao infixa em posfixa
*/
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

/*! empilhaOperando */
/*!
Empilha os operandos
*/
void empilhaOperando(char split[], char posfixa[], t_pilha *pilha, int inicio, int fim){
    int aux = 0, j;

    for(j=inicio; j<fim; j++)
        split[aux++] = posfixa[j];
    split[aux] = '\0';
    push(pilha, atoi(split));
    aux = 0;
}

/*! calculaExpressao. */
/*!
Parte a string atraves dos espaços, e então, empilha, qundo encontra um operador
desempilha os operandos executa a operação e empilha novamente.
Resolve a expressao.
*/
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

/*! resoluçaoExpressao. */
/*!
Junta todos as funcoes de resolucao da expressao desde da validacao da entrada
ate a resolucao da expressao, compondo uma das duas principais funcionalidades
do programa.
*/
void resolucaoExpressao(char expressao[], t_pilha *pilha){
    char posfixa[100];

    if(validaEntrada(expressao)){
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

    }else{
        printf("Expressao invalida\n");
        printf("Pressione ENTER para continuar\n");
        getchar();
        getchar();
    }
}

/*! verificaOperandos. */
/*!
Verifica se ha operandos suficientes para ser feita uma operacao.
*/
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

/*! operacaoBasica. */
/*!
implementa as operacoes basicas de soma, subtracao e multiplicao.
*/
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

/*! operacaoRepeticao. */
/*!
implementa as operacoes de repeticao, usa todos os operandos da pilha empilha
o resultado.
*/
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

/*! copiaDeElemento. */
/*!
implementa a operacao de copia de elemento. retira da pilha a quantidade de
repetiçoes e o elemento que será repetido, executa a operaçao e empilha o
resultado.
*/
void copiaDeElemento(t_pilha *pilha, char entrada[]){
    int i, repeticoes, elemento;

    if(verificaOperandos(pilha)){

        repeticoes = pop(pilha);
        elemento = pop(pilha);

        for(i=1; i<= repeticoes;i++)
            push(pilha, elemento);
    }
}

/*! ValidaCalculadora. */
/*!
permite somente operadores e operandos e a palavra 'sair' para abandonar o modo
calculadora.
*/
int validaCalculadora(char entrada[]){
    int i = 0;

    while(entrada[i] != '\0'){
        if((entrada[i] < 48 || entrada[i] > 57) && strcmp(entrada, "sair") != 0){
            return 0;
        }
        i++;
    }
    return 1;
}

/*! Calculadora. */
/*!
implementa a interface da calculadora e compoe uma das duas funcionalidades do
programa.
*/
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

        }
        else{
            if(validaCalculadora(entrada)){
                valor = atoi(entrada);
                push(pilha, valor);
            }
            else{
                printf("Valor invalido\n");
                printf("Pressione ENTER para continuar ou 'sair' para sair do modo calculadora");
                getchar();
                getchar();
            }
        }
        system(CLEAR);
    }while(strcmp (entrada, "sair") != 0);
}


/*! main. */
/*!
implementa o menu
*/
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

