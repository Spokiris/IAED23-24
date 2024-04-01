/* iaed24 - ist1105865 - system.h */
#ifndef SYSTEM_H
#define SYSTEM_H

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"

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



/// @brief  Função que inicializa o sistema.
/// @param tamanho_hashmap  Tamanho do hashmap
/// @return 
Sistema *inicializa_sistema(int tamanho_hashmap);

/// @brief  Função que liberta o sistema.
/// @param sistema  Sistema
void free_sistema(Sistema *sistema);

/// @brief  Funcao de hash para strings.
/// @param matricula  Matrícula do veículo
/// @param tamanho_hashmap  Tamanho do hashmap
/// @return  Hash da matrícula
int hash_string(const char matricula[SIZMAT], int tamanho_hashmap);

/// @brief  Função que insere um veículo no hashmap.
/// @param sys  Sistema
/// @param veiculo  Veículo
void insere_veiculo(Sistema *sys, Veiculo *veiculo);

/// @brief  Função que procura um veiculo no hashmap.
/// @param sys  Sistema
/// @param matricula  Matrícula do veículo
/// @return  Veículo || NULL se não existir
Veiculo *procura_veiculo(Sistema *sys, char *matricula);

/// @brief  Funcao que inicializa um log na entrada de um veículo.
///         Insere o log na pilha do veículo.
/// @param veiculo  Veículo
/// @param parque  Parque 
/// @param entrada  Data e hora de entrada
/// @return  Log || NULL se não for possível
Log *inicializa_log(Veiculo *veiculo, Parque *parque, char *entrada);

/// @brief  Função que atualiza um log de um veiculo na saída.
///         Adiciona o log a lista de logs do parque.
/// @param parque  
/// @param veiculo 
/// @param saida 
/// @param valor_pago 
void atualiza_log_saida(Parque *parque , Veiculo *veiculo, char *saida, double valor_pago);

/// @brief  Função auxiliar para remover um log de um veículo.
/// @param veiculo  Veículo
/// @param log  Log
void remove_log_veiculo(Veiculo *veiculo, Log *log);

/// @brief  Função que remove um log de um parque.
///         Remove os logs correspondentes dos veiculos.
/// @param sys  Sistema
/// @param parque   Parque
void remove_todos_logs_parque(Sistema *sys, Parque *parque);

/// @brief  Função que lista a pilha de logs de um veiculo.
/// @param veiculo  Veículo
void listar_logs_veiculo(Veiculo *veiculo);

/// @brief  Função que procura um parque no sistema.    
/// @param sys  Sistema
/// @param nome  Nome do parque
/// @return  Parque || NULL se não existir
Parque *procuraParque(Sistema *sys, char *nome);

///@brief Função que valida um parque.
///@param nome Nome do parque
///@param cap Capacidade do parque
///@param X Valor X
///@param Y Valor Y
///@param Z Valor Z
///@return 0 se for válido, 1 se o sistema nao admitir mais parques, 2 se a capacidade for 
///        inválida, 3 se os valores de X, Y ou Z forem inválidos e 4 se o parque já existir
int valida_parque(Sistema *sys, char *nome, int cap, double X, double Y, double Z);

///@brief Função que cria um parque.
///@param sys Sistema
///@param nome Nome do parque
///@param cap Capacidade do parque
///@param X Valor X
///@param Y Valor Y
///@param Z Valor Z
///@return void cria um parque no sistema
void cria_parque(Sistema *sys, char *nome, int cap, double X, double Y, double Z);

/// @brief  Função que imprime os parques por ordem alfabética.
/// @param sys  Sistema
void imprime_parques_alfa(Sistema *sys);

///@brief Função que lista os parques.
///@param sys Sistema 
///@return void lista os parques por ordem de criação
void lista_parques(Sistema *sys);

/// @brief  Função que remove um parque do sistema.
///         Remove os logs correspondentes dos veiculos.
/// @param sys  Sistema
/// @param nome_parque  Nome do parque
void remove_parque(Sistema *sys, char *nome_parque);

/// @brief  Função que valida uma entrada.
/// @param sys  Sistema
/// @param nomeParque  Nome do parque
/// @param matricula  Matrícula do veículo
/// @param data  Data e hora de entrada
/// @return  0 se for válida, 1 se o parque não existir, 2 se o veículo já estiver no parque,
///          3 se a matrícula for inválida, 4 se a data for inválida
int valida_entrada(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]);

/// @brief  Função que adiciona um veículo a um parque.
/// @param sys  Sistema
/// @param nomeParque  Nome do parque
/// @param matricula  Matrícula do veículo
/// @param data  Data e hora de entrada
void adiciona_veiculo(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]);

/// @brief  Função que calcula a tarifa de um veículo.
/// @param parque  Parque
/// @param dataE  Data e hora de entrada
/// @param dataS  Data e hora de saída
/// @return  Tarifa
double calcula_tarifa(Parque *parque, char *dataE, char *dataS);

/// @brief  Função que insere a faturacao de um veiculo no parque.
/// @param parque  Parque
/// @param dia  Dia
/// @param valor    Valor
void insere_faturacao(Parque *parque, char *dia, float valor);

/// @brief  Função que valida uma saída.
/// @param sys  Sistema
/// @param nomeParque  Nome do parque
/// @param matricula   Matrícula do veículo
/// @param data  Data e hora de saída
/// @return  0 se for válida, 1 se o parque não existir, 2 se o veículo não estiver no parque,
///          3 se a matrícula for inválida, 4 se a data for inválida
int valida_saida(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]);

/// @brief   Função que remove um veículo de um parque.
/// @param sys  Sistema
/// @param nomeParque  Nome do parque
/// @param matricula  Matrícula do veículo
/// @param data  Data e hora de saída
void remove_veiculo(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]);

/// @brief  Função que lista a faturação de um parque.
/// @param sys  Sistema
/// @param nomeParque  Nome do parque
/// @param data     Data
void lista_faturacao(Sistema *sys, char *nomeParque, char *data);

#endif // SYSTEM_H