/* iaed24 - ist1105865 - parser.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"


int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}


int finalInput() {
    int c = getchar();
    if (c != '\n')  return 1;
    return 0;
}


void leNome(char **s) {
    int i = 0, c;
    (*s)[0] = getchar();
    if ((*s)[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n') {
            (*s)[i++] = c;
        }
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"') {
            if(c == '\n') {
                (*s)[i] = '\0';
                return;
            }
            (*s)[i++] = c;
        }
    }
    (*s)[i] = '\0';

    if (i > 0 && (*s)[0] == ' ') {
        memmove(*s, *s + 1, i);
    }
    return;
}


void leData(char data[MAXDATA]) {
    int i = 0, c;

    while ((c = getchar()) != '\t' && c != '\n') {
        data[i++] = c;
    }
    data[i] = '\0';

    if (strlen(data) == 15) {    
        // Adiciona um zero ao minuto se a hora for menor que 10
        for (i = strlen(data); i >= 11; i--) {
            data[i] = data[i - 1];
        }
        data[11] = '0';
    }
}


void leDia(char dia[10]) {
    
    int i = 0, c;
    while ((c = getchar()) == ' ' || c == '\t');
    
    if (c == '\n') {
        dia[0] = '\0';
        return;
    }

    dia[i++] = c;
    while ((c = getchar()) != ' ' && c != '\t' && c != '\n') {
        dia[i++] = c;
    }
    dia[i] = '\0';

}


void leMatricula(char matricula[SIZMAT]) {
    int i = 0, c;
    // Ignora espaços em branco até encontrar o início da matrícula
    while ((c = getchar()) == ' ' || c == '\t' || c == '\n');

    matricula[i++] = c; // Armazena o primeiro caractere na matrícula

    while ((c = getchar()) != ' ' && c != EOF && c != '\n') {
        // Armazena o caractere na matrícula
        if (i < SIZMAT-1) { // Verifica se há espaço suficiente na matrícula
            matricula[i++] = c;
        } 

    }

    matricula[i] = '\0'; // Adiciona o terminador nulo
    return;
}


int AnoBissexto(int ano) {
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        return 1;
    else
        return 0;
}


int valida_dia(int dia, int mes, int ano) {
    // Verifica se o ano é válido (entre 1 e 9999)
    if (ano < 1 || ano > 9999)
        return 1;

    // Verifica se o mês é válido (entre 1 e 12)
    if (mes < 1 || mes > 12)
        return 1;

    // Verifica se o dia é válido de acordo com o mês
    switch (mes) {
        case 1: // Janeiro
        case 3: // Março
        case 5: // Maio
        case 7: // Julho
        case 8: // Agosto
        case 10: // Outubro
        case 12: // Dezembro
            if (dia < 1 || dia > 31) return 1;
            break;
        case 4: // Abril
        case 6: // Junho
        case 9: // Setembro
        case 11: // Novembro
            if (dia < 1 || dia > 30) return 1;
            break;
        case 2: // Fevereiro
            if (AnoBissexto(ano)) {
                if (dia < 1 || dia > 29) return 1;
            } else {
                if (dia < 1 || dia > 28) return 1;
            }
            break;
        default:
            return 1;
    }
    return 0; // Retorna 0 se a data for válida
}


int valida_hora(int hora, int minuto) {
    // Verifica se a hora está no intervalo válido (0-23 horas) e se os minutos estão no intervalo válido (0-59)
    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) return 1; 
    return 0; 
}



int valida_data(char data[MAXDATA]) {
    char dia[3], mes[3], ano[5], hora[3],minuto[3];

    if (sscanf(data, "%2s-%2s-%4s %2s:%2s", dia, mes, ano, hora, minuto) != 5) {
        return 1;
    }


    if (data[2] != '-' || data[5] != '-' || data[10] != ' ' || (data[12] != ':' && data[13] != ':')) {
        
        return 1;
    }
    int diaI = atoi(dia);
    int mesI = atoi(mes);
    int anoI = atoi(ano);
    int horaI = atoi(hora); 
    int minutoI = atoi(minuto);


    if (valida_dia(diaI, mesI, anoI) || valida_hora(horaI, minutoI)) {

        return 1;
    }

    return 0;

}


int eh29deFev(char data[MAXDATA]) {
    int dia, mes, ano, hora, minuto;
    sscanf(data, "%d-%d-%d %d:%d", &dia, &mes, &ano, &hora, &minuto);
    if (mes == 2 && dia == 29) {
        if (AnoBissexto(ano)) {
            return 1; // É 29 de fevereiro em um ano bissexto
        }
    }
    return 0;
}


int comp_data(char dataEntrada[MAXDATA], char dataUltima[MAXDATA]) {
    
    if(dataUltima[0] == '\0') return 0;
    
    char cdia[3], cmes[3], cano[5], chora[3], cminuto[3];
    char cdiaU[3], cmesU[3], canoU[5], choraU[3], cminutoU[3];
    
    if (sscanf(dataEntrada, "%2s-%2s-%4s %2s:%2s", cdia, cmes, cano, chora, cminuto) != 5) {

        return 1;
    }

    if (sscanf(dataUltima, "%2s-%2s-%4s %2s:%2s", cdiaU, cmesU, canoU, choraU, cminutoU) != 5) {

        return 1;
    }

    int dia = atoi(cdia);
    int mes = atoi(cmes);
    int ano = atoi(cano);
    int hora = atoi(chora);
    int minuto = atoi(cminuto);

    int diaU = atoi(cdiaU);
    int mesU = atoi(cmesU);
    int anoU = atoi(canoU);
    int horaU = atoi(choraU);
    int minutoU = atoi(cminutoU);


    if (ano < anoU || (ano == anoU && mes < mesU) || (ano == anoU && mes == mesU && dia < diaU) ||
        (ano == anoU && mes == mesU && dia == diaU && hora < horaU) ||
        (ano == anoU && mes == mesU && dia == diaU && hora == horaU && minuto < minutoU)) {
        
        return 1;
    }
    
    return 0;
}


int valida_matricula(char matricula[SIZMAT]) {
    int tem_letras = 0, tem_numeros = 0;

    if (strlen(matricula) != SIZMAT - 1) {
        return 1;
    }
    if(matricula[2] != '-' || matricula[5] != '-') {
        return 1;
    }

    for (int i = 0; i < SIZMAT - 1; i += 3) {
        if (isalpha(matricula[i]) && isalpha(matricula[i+1])) {
            tem_letras = 1;
        } else if (isdigit(matricula[i]) && isdigit(matricula[i+1])) {
            tem_numeros = 1;
        } else {
            return 1;
        }
    }

    if (!tem_letras || !tem_numeros) {
        return 1;
    }

    return 0; // Retorna 0 se a matrícula for válida
}


int dias_no_mes(int mes, int ano) {
    if (mes == 2) {
        if (AnoBissexto(ano)) {
            return 29; // Fevereiro em um ano bissexto
        } else {
            return 28; // Fevereiro em um ano não bissexto
        }
    } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30; // Meses com 30 dias
    } else {
        return 31; // Meses com 31 dias
    }
}


int calcular_minutos_total(int dia, int mes, int ano, int hora, int minuto) {
    int minutos = 0;
    // COMO DIA 29/02 O PARQUE ESTA SEMPRE FECHADO
    // TODOS OS ANOS TEM UM TEMPO UTIL DE 365 DIAS
    
    minutos += ano * 365 * 24 * 60;

    // Adiciona minutos dos meses completos
    for (int i = 1; i < mes; i++) {
        minutos += dias_no_mes(i, ano) * 24 * 60;
    }

    // Adiciona minutos dos dias completos
    minutos += (dia - 1) * 24 * 60;

    // Adiciona minutos das horas completas
    minutos += hora * 60;

    // Adiciona minutos
    minutos += minuto;

    return minutos;
}


int calcula_minutos_passados(char *data1, char *data2) {
    int dia1, mes1, ano1, hora1, minuto1;
    sscanf(data1, "%d-%d-%d %d:%d", &dia1, &mes1, &ano1, &hora1, &minuto1);

    int dia2, mes2, ano2, hora2, minuto2;
    sscanf(data2, "%d-%d-%d %d:%d", &dia2, &mes2, &ano2, &hora2, &minuto2);

    // Calcula a quantidade total de minutos para cada data
    int total_minutos_data1 = calcular_minutos_total(dia1, mes1, ano1, hora1, minuto1);
    int total_minutos_data2 = calcular_minutos_total(dia2, mes2, ano2, hora2, minuto2);

    // Calcula a diferença entre as quantidades totais de minutos
    int diferenca_minutos = abs(total_minutos_data2 - total_minutos_data1);

    return diferenca_minutos;
}

char *horaP(char* str) {
    int len = strlen(str);
    if (len < 5) {
        return NULL;
    }
    return &str[len - 5];
}


char *dataP(char* str) {
    char *primeiros_10 = malloc(11 * sizeof(char));
    if (primeiros_10 == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }
    strncpy(primeiros_10, str, 10);
    primeiros_10[10] = '\0'; 
    return primeiros_10;
}
