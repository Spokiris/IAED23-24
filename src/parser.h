/* iaed24 - ist1105865 - parser.h */
#ifndef PARSER_H
#define PARSER_H

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"

/// @brief  Função que lê espaços
/// @return  1 se houver espaços, 0 caso contrário
int leEspacos();

/// @brief  Função que verifica se o input terminou
/// @return  1 se o input terminou, 0 caso contrário
int finalInput();

/// @brief  Função que lê o nome
/// @param s  Nome
void leNome(char **s);

/// @brief  Função que lê dia e hora {dd-mm-aaaa hh:mm}
/// @param data  Data
void leData(char data[MAXDATA]);

/// @brief  Função que lê o dia {dd-mm-aaaa}
/// @param dia  Dia
void leDia(char dia[10]);

/// @brief  Função que lê a matrícula
/// @param matricula  Matrícula
void leMatricula(char matricula[SIZMAT]);

/// @brief  Função que verifica se uma ano e bissexto
/// @param ano  Ano
/// @return  1 se for bissexto, 0 caso contrário
int AnoBissexto(int ano);

/// @brief  Função que verifica se um dia é válido
/// @param dia  Dia
/// @param mes  Mês
/// @param ano  Ano
/// @return  1 se for válido, 0 caso contrário
int valida_dia(int dia, int mes, int ano);

/// @brief  Função que verifica se uma hora é válida
/// @param hora  Hora
/// @param minuto Minuto
/// @return  1 se for válida, 0 caso contrário
int valida_hora(int hora, int minuto);

/// @brief  Função que verifica se uma data é válida
/// @param data  Data
/// @return  1 se for válida, 0 caso contrário
int valida_data(char data[MAXDATA]);

/// @brief  Função que verifica se e dia 29/02 
/// @param data  Data
/// @return   1 se for válida, 0 caso contrário
int eh29deFev(char data[MAXDATA]);

/// @brief  Função que compara duas datas
/// @param dataEntrada  Data de entrada
/// @param dataUltima  Data da ultima entrada
/// @return  1 se a data de entrada for postrior a ultima entrada, 0 caso contrário
int comp_data(char dataEntrada[MAXDATA], char dataUltima[MAXDATA]);

/// @brief  Função que valida uma matrícula
/// @param matricula  Matrícula
/// @return  1 se for válida, 0 caso contrário
int valida_matricula(char matricula[SIZMAT]);

/// @brief  Função que valida uma entrada
/// @param mes  Mês
/// @param ano  Ano
/// @return  1 se for válida, 0 caso contrário
int dias_no_mes(int mes, int ano);

/// @brief  Função que calcula o total de minutos
/// @param dia  Dia
/// @param mes  Mês
/// @param ano  Ano
/// @param hora  Hora
/// @param minuto  Minuto
/// @return  Total de minutos
int calcular_minutos_total(int dia, int mes, int ano, int hora, int minuto);

/// @brief   Função que calcula os minutos passados
/// @param data1  Primeira data
/// @param data2  Segunda data
/// @return  Minutos passados
int calcula_minutos_passados(char *data1, char *data2);

/// @brief  Retira uma hora de uma data
/// @param str  Data
/// @return  Hora
char *horaP(char* str);

/// @brief Retira um dia de uma data
/// @param str  Data
/// @return  Dia
char *dataP(char* str);

#endif // PARSER_H



