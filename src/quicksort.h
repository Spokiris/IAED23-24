/* iaed24 - ist1105865 - quicksort.h */
#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"

/// @brief  Função que troca dois logs de posição
/// @param logs  Array de logs
/// @param i  Índice do primeiro log
/// @param j  Índice do segundo log
void swap_logs(Log **logs, int i, int j);

/// @brief  Função que divide o array de logs em duas partes
/// @param logs  Array de logs
/// @param low  Índice do primeiro log
/// @param high  Índice do último log
/// @return  Índice do log que será o pivot
int partition_logs(Log **logs, int low, int high);

/// @brief  Função que ordena o array de logs
/// @param logs  Array de logs
/// @param low  Índice do primeiro log
/// @param high  Índice do último log
void quicksort_logs(Log **logs, int low, int high);

#endif
