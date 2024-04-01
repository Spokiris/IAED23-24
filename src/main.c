/* iaed24 - ist1105865 - main.c */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"


int main() {
    Sistema *sys = inicializa_sistema(478376);
    while(1) {
        input_handler(sys);
    }
    free_sistema(sys);
    return 0; 
}

