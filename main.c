#include <stdio.h>
#include "pilha.h"

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

int main(){
    char expressao[100], posfixa[100];
    t_pilha* pilha = getPilha(10);

    if(!validaExpressao(expressao, pilha))
        printf("invalido\n");
    else{
        posFixa(expressao, posfixa, pilha);
        printf("%s\n", posfixa);
    }


    liberaPilha(pilha);

    return 0;
}
