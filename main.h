#ifndef MAIN_H
#define MAIN_H

#define MAXPARQUES 20
#define BUFFSIZ 8129
#define MAXDATA 17
#define SIZMAT 9

typedef struct Log {
    char *matricula;                // Matrícula do veículo
    char *nome_p;                   // Nome do parque

    char entrada[MAXDATA];          // Data e hora de entrada
    char saida[MAXDATA];            // Data e hora de saída

    double valor_p;                 // Valor pago pelo período de estacionamento
    struct Log *nextp;              // Próximo log do parque
    struct Log *prevV;              // Log anterior do veículo
} Log;

typedef struct Faturacao {
    char dia[MAXDATA];              // Dia
    float valor;                    // Valor total faturado no dia
    struct Faturacao *next;         // Próximo dia
} Faturacao;

typedef struct Veiculo{ 
    int inside;                     // 1 se o veículo está dentro de um parque, 0 caso contrário
    char matricula[SIZMAT];         // Matrícula do veículo
    Log *toplog;                    // Log do veículo
    struct Veiculo *next;           // Próximo veículo em caso de colisa
} Veiculo;

typedef struct Parque {
    char *nome;                     // Nome do parque
    int capacidade;                 // Capacidade do parque
    double X;                       // Valor da Tarifa nos quartos de hora da primeira hora
    double Y;                       // Valor da Tarifa nos quartos de hora das horas seguintes
    double Z;                       // Valor da Tarifa maxima diaria
    unsigned int lugares_livres;    // Lugares livres no parque
    Log *headlog;                   // Log do parque
    struct Parque *next;            // Próximo parque
    struct Parque *prev;            // Parque anterior
    char current[MAXDATA];          // Data e hora atual
    Faturacao *faturacao;           // Faturação do parque
} Parque;

typedef struct Sistema {    
    Parque *headparque;             // Lista de parques
    Veiculo **Vhashmap;             // Hashmap de veículos
    int hashmap_size;               // Tamanho do hashmap
    int num_parques;                // Número de parques
} Sistema;


#endif
