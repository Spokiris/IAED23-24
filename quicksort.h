#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "main.h"

void swap_logs(Log **logs, int i, int j);
int partition_logs(Log **logs, int low, int high);
void quicksort_logs(Log **logs, int low, int high);

#endif
