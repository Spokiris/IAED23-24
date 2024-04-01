/* iaed24 - ist1105865 - quicksort.c */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"

void swap_logs(Log **logs, int i, int j) {
    Log *temp = logs[i];
    logs[i] = logs[j];
    logs[j] = temp;
}

int partition_logs(Log **logs, int low, int high) {
    
    if (logs == NULL) {
        printf("Invalid logs.\n");
        return -1;
    }
    
    Log *pivot = logs[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // Comparação: primeiro por nome do parque e depois por data de entrada
        if (strcmp(logs[j]->nome_p, pivot->nome_p) < 0 ||
            (strcmp(logs[j]->nome_p, pivot->nome_p) == 0 && strcmp(logs[j]->entrada, pivot->entrada) < 0)) {
            i++;
            swap_logs(logs, i, j);
        }
    }

    swap_logs(logs, i + 1, high);
    return i + 1;
}

void quicksort_logs(Log **logs, int low, int high) {
    
    if (low < high) {
        int pi = partition_logs(logs, low, high);

        quicksort_logs(logs, low, pi - 1);
        quicksort_logs(logs, pi + 1, high);
    }
}


