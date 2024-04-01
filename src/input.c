/* iaed24 - ist1105865 - input.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"


void in_hand_q() {
    exit(0);
    return;
}

void in_hand_p(Sistema *sys) {
    char *nome = NULL; 
    int cap;
    double X, Y, Z;
    int fimLinha = leEspacos();

    if (!fimLinha){ 
        lista_parques(sys); 
        return;
    }


    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); 
    scanf("%d", &cap);
    if (scanf("%lf %lf %lf", &X, &Y, &Z) != 3) {
        printf("invalid args.\n");
        free(nome);
        return;
    } 

    if(finalInput()) {
        printf("invalid args.\n");
        free(nome);
        return;
    }
    cria_parque(sys, nome, cap, X, Y, Z);


    free(nome);
    return;
}


void in_hand_e(Sistema *sys) {
    char *nome = NULL;
    char matricula[SIZMAT];
    char data[MAXDATA] = "";
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }


    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); 
    leMatricula(matricula);
    leData(data);
    adiciona_veiculo(sys, nome, matricula, data);
    free(nome);
    return;
}   


void in_hand_s(Sistema *sys) {
    char *nome = NULL;
    char matricula[SIZMAT];
    char data[MAXDATA] = "";
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }


    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); 
    leMatricula(matricula);
    leData(data);
    remove_veiculo(sys, nome, matricula, data);
    free(nome);
}


void in_hand_v(Sistema *sys) {
    char matricula[SIZMAT];
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }

    leMatricula(matricula);

    if(valida_matricula(matricula)) {
        printf("%s: invalid licence plate.\n", matricula);
        return;
    }
    
    Veiculo *veiculo = NULL;
    veiculo = procura_veiculo(sys, matricula);
    if (veiculo == NULL || veiculo->toplog == NULL) {
        printf("%s: no entries found in any parking.\n", matricula);
        return;
    } 

    listar_logs_veiculo(veiculo);
    
    return;
}


void in_hand_f(Sistema *sys) {
    char *nome = NULL;
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }


    nome = (char *)malloc(BUFFSIZ * sizeof(char));

    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome);
 
    char *dia = NULL;
    dia = (char *)malloc(BUFFSIZ * sizeof(char));
    leDia(dia);
    lista_faturacao(sys, nome, dia);
    free(nome);
    free(dia);
    return;
}


void in_hand_r(Sistema *sys) {
    char *nome = NULL;
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }

    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); 
    remove_parque(sys, nome);
    free(nome);
    return;
} 

void input_handler(Sistema *sys) {

    int c;
    c = getchar();
    switch (c) {
        case 'q':
            in_hand_q();
            break;
        case 'p':
            in_hand_p(sys);
            break;
        case 'e':
            in_hand_e(sys);
            break;
        case 's':
            in_hand_s(sys);
            break;
        case 'v':
            in_hand_v(sys);
            break;
        case 'f':
            in_hand_f(sys);
            break;
        case 'r':
            in_hand_r(sys);
            break;
        default:
            if (c == ' ' || c == '\t' || c == '\n') break;
            break;
    }
    return;
}


