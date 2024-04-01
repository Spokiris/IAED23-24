/* iaed24 - ist1105865 - system.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"

Sistema *inicializa_sistema(int tamanho_hashmap) {
    Sistema *sys = (Sistema *)malloc(sizeof(Sistema));
    if (sys == NULL) {
        printf("Erro ao alocar memoria para o sistema.\n");
        exit(1);
    }
    sys->headparque = NULL;
    sys->num_parques = 0;
    sys->hashmap_size = tamanho_hashmap;
    sys->Vhashmap = (Veiculo **)malloc(tamanho_hashmap * sizeof(Veiculo *));
    if (sys->Vhashmap == NULL) {
        printf("Erro ao alocar memoria para o hash map de veiculos.\n");
        exit(1);
    }
    for (int i = 0; i < tamanho_hashmap; i++) {
        sys->Vhashmap[i] = NULL;
    }
    return sys;
}

void free_sistema(Sistema *sys) {
    if (sys != NULL) {
        // Liberta os parques
        Parque *parque_atual = sys->headparque;
        while (parque_atual != NULL) {
            Parque *parque_proximo = parque_atual->next;
            free(parque_atual);
            parque_atual = parque_proximo;
        }
        // Liberta o hash map de veículos
        free(sys->Vhashmap);
        // Liberta a estrutura do sistema
        free(sys);
        return;
    }
}


int hash_string(const char matricula[SIZMAT], int tamanho_hashmap) {
    int hash = 0;
    if (matricula != NULL) {
        for (int i = 0; matricula[i] != '\0'; i++) {
            hash = (hash * 31 + matricula[i]) % tamanho_hashmap;
        }
    }
    return hash;
}



void insere_veiculo(Sistema *sys, Veiculo *veiculo) {
    
    int indice = hash_string(veiculo->matricula, sys->hashmap_size);


    if (sys->Vhashmap[indice] == NULL) {

        sys->Vhashmap[indice] = veiculo;
        veiculo->next = NULL;
    } else { // Colisão

        Veiculo *atual = sys->Vhashmap[indice]; 
        while(atual->next != NULL) {
            atual = atual->next; 
        }
        atual->next = veiculo;
        veiculo->next = NULL;
    }
}



Veiculo *procura_veiculo(Sistema *sys, char *matricula) {

    if (sys == NULL) {
        printf("Invalid system.\n");
        return NULL;
    }

    int indice = hash_string(matricula, sys->hashmap_size);

    Veiculo *veiculo = sys->Vhashmap[indice];

    if (veiculo == NULL) return NULL;

    if (strcmp(veiculo->matricula, matricula) == 0) {
        return veiculo; 
    } else { // Colisão
       
        while (veiculo->next != NULL) {
            veiculo = veiculo->next; 
            if (strcmp(veiculo->matricula, matricula) == 0) {
                return veiculo; 
            }
        }
    }
    return NULL;
}

Log *inicializa_log(Veiculo *veiculo, Parque *parque, char *entrada) {
    if (veiculo == NULL || parque == NULL || entrada == NULL) {
        return NULL; 
    }


    Log *novo_log = (Log *)malloc(sizeof(Log));
    if (novo_log == NULL) {
        printf("Erro ao alocar memoria para o log.\n");
        return NULL;
    }
    

    novo_log->matricula = (char *)malloc((strlen(veiculo->matricula) + 1) * sizeof(char));
    if (novo_log->matricula == NULL) {
        printf("Erro ao alocar memoria para a matrícula do veículo no log.\n");
        free(novo_log);
        return NULL;
    }
    strcpy(novo_log->matricula, veiculo->matricula);
    

    novo_log->nome_p = (char *)malloc((strlen(parque->nome) + 1) * sizeof(char));
    if (novo_log->nome_p == NULL) {
        printf("Erro ao alocar memoria para o nome do parque no log.\n");
        free(novo_log->matricula);
        free(novo_log);
        return NULL;
    }
    strcpy(novo_log->nome_p, parque->nome);
    
    // Copia a entrada
    strcpy(novo_log->entrada, entrada);
    novo_log->saida[0] = '\0'; // Inicializa a saída como vazia
    novo_log->valor_p = 0.0; // Inicializa o valor pago como 0
    novo_log->nextp = NULL; // Define o próximo log como NULL
    
    // Atualiza o topo do log do veículo 
    novo_log->prevV = veiculo->toplog;
    veiculo->toplog = novo_log;
    
    return novo_log;
}


void atualiza_log_saida(Parque *parque , Veiculo *veiculo, char *saida, double valor_pago) {
    
    if (veiculo == NULL || saida == NULL || veiculo->toplog == NULL ) return;
    
    strcpy(veiculo->toplog->saida, saida);
    veiculo->toplog->valor_p = valor_pago;

    if (parque != NULL) {
        if (parque->headlog == NULL) {
            parque->headlog = veiculo->toplog;
        } else {
            Log *current_log = parque->headlog;
            while (current_log->nextp != NULL) {
                current_log = current_log->nextp;
            }
            current_log->nextp = veiculo->toplog;
        }
    } else {
        printf("Erro: Parque nao encontrado para adicionar o log.\n");
    }
}

void remove_log_veiculo(Veiculo *veiculo, Log *log) {
    if (veiculo == NULL || log == NULL) return;

    if (veiculo->toplog == NULL) {
        printf("Erro: Pilha de logs do veiculo vazia.\n");
        return;
    }

    // se o log a ser removido for o topo da pilha
    if (veiculo->toplog == log) {
        veiculo->toplog = log->prevV; 
        free(log->matricula); 
        free(log); 
        return;
    }

    // Procura o log na pilha do veículo
    Log *prev_log = veiculo->toplog;
    while (prev_log->prevV != NULL && prev_log->prevV != log) {
        prev_log = prev_log->prevV;
    }

    // Remove o log da pilha do veículo
    if (prev_log->prevV == log) {
        prev_log->prevV = log->prevV;
        free(log->matricula); 
        free(log); 
    } else {
        printf("Erro: Log nao encontrado no veiculo.\n");
    }
}


void remove_todos_logs_parque(Sistema *sys, Parque *parque) {
    
    if (parque == NULL) return;
    
    Log *log = parque->headlog;
    while (log != NULL) {
        if (log->nome_p != NULL) {

            log->nome_p = NULL;

            Veiculo *veiculo = procura_veiculo(sys, log->matricula);
            if (veiculo != NULL) {
                
                if (veiculo->inside == 1 && strcmp(parque->nome, veiculo->toplog->nome_p) == 0) {
                    // caso o veículo ainda esteja dentro do parque
                    Log *temp = veiculo->toplog;
                    veiculo->toplog = veiculo->toplog->prevV;
                    free(temp->matricula);
                    free(temp);
                    veiculo->inside = 0; 
                }
                remove_log_veiculo(veiculo, log);
            }
        }
        log = log->nextp;
    }
}


void listar_logs_veiculo(Veiculo *veiculo) {
    if (veiculo == NULL) return;

    Log *temp_log = veiculo->toplog;


    Log **logs = NULL;
    int num_logs = 0; 

    // Armazenar os logs do veiculo num array dinâmico
    while (temp_log != NULL) {
        Log **temp = realloc(logs, (num_logs + 1) * sizeof(Log *));
        if (temp == NULL) {
            printf("Erro de alocaçao de memoria.\n");
            free(logs); 
            return;
        }
        logs = temp;
        logs[num_logs++] = temp_log;
        temp_log = temp_log->prevV;
    }

    quicksort_logs(logs, 0, num_logs - 1); // sort


    for (int i = 0; i < num_logs; i++) {
        printf("%s %s", logs[i]->nome_p, logs[i]->entrada);
        if (logs[i]->saida[0] != '\0') {
            printf(" %s", logs[i]->saida);
        }
        printf("\n");
    }

    free(logs);
}

Parque *procuraParque(Sistema *sys, char *nome) {
    if (sys == NULL) {
        printf("Invalid system.\n");
        return NULL;
    }
    
    Parque *head = sys->headparque;
    while(head != NULL) {
    if(head->nome != NULL && strcmp(head->nome, nome) == 0) {
        return head;
    }
    head = head->next;
}
    return NULL;
}


int valida_parque(Sistema *sys, char *nome, int cap, double X, double Y, double Z) {
    if(sys->num_parques >= MAXPARQUES) return 1;
    if(procuraParque(sys, nome) != NULL) return 2;
    if(cap <= 0) return 3;
    if ( X > Y || Y > Z || X <= 0 || Y <= 0 || Z <= 0) return 4;
    return 0;
}


void cria_parque(Sistema *sys, char *nome, int cap, double X, double Y, double Z) {
    Parque *parque = NULL;
    parque = (Parque *)malloc(sizeof(Parque));
    if (parque == NULL) {
        printf("Erro ao alocar memoria para o parque.\n");
        free(parque);
        return;
    }
    switch (valida_parque(sys, nome, cap, X, Y, Z))
    {
    case 0:

        parque->nome = strdup(nome);
        if(parque->nome == NULL) {
            printf("Erro ao alocar memoria para o nome do parque.\n");
            free(parque);
            return;
        }
        parque->capacidade = cap;
        parque->lugares_livres = cap;
        parque->X = X;
        parque->Y = Y;
        parque->Z = Z;
        parque->headlog = NULL;
        parque->current[0] = '\0';
        parque->next = NULL;
        parque->prev = NULL;
        parque->faturacao = NULL;
        
        if (sys->headparque == NULL) {
                sys->headparque = parque;  // Se a lista estiver vazia, o novo parque se torna o primeiro
            } else {
                // Procurar o último parque na lista
                Parque *head = sys->headparque;
                while (head->next != NULL) {
                    head = head->next;
                }
                // Adicionar o novo parque após o último parque na lista
                head->next = parque;
                parque->prev = head;
            }
            sys->num_parques++;

            break;

    case 1:
        printf("too many parks.\n");
        free(parque);
        break;
    
    case 2:
        printf("%s: parking already exists.\n", nome);
        free(parque);
        break;
    
    case 3:
        printf("%d: invalid capacity.\n", cap);
        free(parque);
        break;

    case 4:
        printf("invalid cost.\n");
        free(parque);
        break;
    
    default:
        break;
    }
    return;
}


void imprime_parques_alfa(Sistema *sys) {

    char *nomes[MAXPARQUES];
    int num_parques = 0;

    Parque *atual = sys->headparque;
    while (atual != NULL && num_parques < MAXPARQUES) {
        nomes[num_parques] = atual->nome;
        atual = atual->next;
        num_parques++;
    }

    // Ordena o vetor de nomes dos parques por ordem alfabetica
    for (int i = 0; i < num_parques - 1; i++) {
        for (int j = i + 1; j < num_parques; j++) {
            if (strcmp(nomes[i], nomes[j]) > 0) {
                char *temp = nomes[i];
                nomes[i] = nomes[j];
                nomes[j] = temp;
            }
        }
    }

    for (int i = 0; i < num_parques; i++) {
        printf("%s\n", nomes[i]);
    }
}


void lista_parques(Sistema *sys) {
    Parque *current = sys->headparque;
    while (current != NULL) {
        printf("%s %d %u\n", current->nome, current->capacidade, current->lugares_livres);
        current = current->next;
    }
    return;
}


void remove_parque(Sistema *sys, char *nome_parque) {
    Parque *parque = NULL;
    parque = procuraParque(sys, nome_parque);

    if (parque == NULL) {
        printf("%s: no such parking.\n", nome_parque);
        return;
    }
    
    if (parque->prev != NULL)
        parque->prev->next = parque->next;
    if (parque->next != NULL)
        parque->next->prev = parque->prev;

    if (sys->headparque == parque)
        sys->headparque = parque->next; 
    sys->num_parques--;
    
    remove_todos_logs_parque(sys, parque);
    
    while(parque->faturacao != NULL) {
        Faturacao *temp = parque->faturacao;
        parque->faturacao = parque->faturacao->next;
        free(temp);
    }
    free(parque->nome); 
    free(parque); 
    imprime_parques_alfa(sys);
    return;
}


int valida_entrada(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]) {
    Parque *parque = NULL;  
    parque = procuraParque(sys, nomeParque);
    if (parque == NULL) return 1;
    if (parque->lugares_livres == 0) return 2;
    if (valida_matricula(matricula)) return 3;
    Veiculo *veiculo = NULL;
    veiculo = procura_veiculo(sys, matricula);
    if (veiculo != NULL && veiculo->inside == 1) return 4;
    if (valida_data(data) || comp_data (data, parque->current)|| eh29deFev(data)) return 5;
    
    return 0;
}


void adiciona_veiculo(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]) {
    switch (valida_entrada(sys, nomeParque, matricula, data))
    {
    case 0:
        Veiculo *veiculo = procura_veiculo(sys, matricula);
        if (veiculo == NULL) {
            veiculo = (Veiculo *)malloc(sizeof(Veiculo));
            if (veiculo == NULL) {
                printf("Erro ao alocar memória para o veículo.\n");
                return;
            }
            veiculo->toplog = NULL; // Inicialize a pilha de logs do veiculo
            strcpy(veiculo->matricula, matricula);
            veiculo->inside = 0;
            insere_veiculo(sys, veiculo);
        }
    
        veiculo->inside = 1;
        Parque *parque = procuraParque(sys, nomeParque);
        if (parque != NULL) {
            parque->lugares_livres--;
            strcpy(parque->current, data);
            inicializa_log(veiculo, parque, data);
            printf("%s %u\n", nomeParque, parque->lugares_livres);
            return;
        } else {
            printf("%s: no such parking.\n", nomeParque);
            return;
        }
        break;
    case 1:
        printf("%s: no such parking.\n", nomeParque);
        break;

    case 2:
        printf("%s: parking is full.\n", nomeParque);
        break;

    case 3:
        printf("%s: invalid licence plate.\n", matricula);
        break;

    case 4:
        printf("%s: invalid vehicle entry.\n", matricula);
        break;
    case 5:
        printf("invalid date.\n"); 
        break;
    default: 
        break;

    }
    return;
}


double calcula_tarifa(Parque *parque, char *dataE, char *dataS) {
    
    double X = parque->X;
    double Y = parque->Y;
    double Z = parque->Z;

    long int minutos = calcula_minutos_passados(dataE, dataS);
    
    double tarifa = 0;

    double tarifaX = 0;
    double tarifaY = 0;
    double tarifaZ = 0;
    
    int dias = minutos / 1440;
    
    if(dias > 0) {
        minutos -= (dias * 1440);
        tarifaZ += dias * Z;
    }
    
    long int minresto = minutos;
 
   if (minresto > 60) {
        tarifaX += X * 4;
        minresto -= 60;
        tarifaY += (minresto % 15 == 0) ? (minresto / 15) * Y : ((minresto / 15) + 1) * Y;
    } else {
        tarifaX += (minresto % 15 == 0) ? (minresto / 15) * X : ((minresto / 15) + 1) * X;
    }
    
    if(tarifaX + tarifaY >= Z) {
        tarifaX = 0;
        tarifaY = 0;
        tarifaZ += Z;
    }

    
    tarifa = tarifaX + tarifaY + tarifaZ;

    return tarifa;
}

void insere_faturacao(Parque *parque, char *dia, float valor) {
    
    if (parque == NULL) {
        printf("Invalid park.\n");
        return;
    }
    
    if (parque->faturacao == NULL) {
        parque->faturacao = (Faturacao *)malloc(sizeof(Faturacao));
        if (parque->faturacao == NULL) {
            printf("Erro ao alocar memoria para a faturaçao.\n");
            return;
        }

        strcpy(parque->faturacao->dia, dia);
        parque->faturacao->valor = valor;
        parque->faturacao->next = NULL;
    } else {

        Faturacao *current = parque->faturacao;
        while (current != NULL && current->next != NULL && strcmp(current->dia, dia) != 0) {
            current = current->next;
        }

        if (current != NULL && strcmp(current->dia, dia) == 0) {

            current->valor += valor;
        } else {

            if (current != NULL) {
                current->next = (Faturacao *)malloc(sizeof(Faturacao));
                if (current->next == NULL) {
                    printf("Erro ao alocar memoria para a faturaçao.\n");
                    return;
                }

                current = current->next;
                strcpy(current->dia, dia);
                current->valor = valor;
                current->next = NULL;
            } else {
                printf("Erro: Lista de faturaçao nao inicializada corretamente.\n");
            }
        }
    }
}


int valida_saida(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]) {
    Parque *parque;
    parque = procuraParque(sys, nomeParque);
    Veiculo *veiculo = NULL;
    veiculo = procura_veiculo(sys, matricula);

    if (parque == NULL) return 1;
    if (valida_matricula(matricula)) return 2;
    if (veiculo == NULL || veiculo->inside == 0 || strcmp(veiculo->toplog->nome_p, nomeParque) != 0 ) return 3;
    if (valida_data(data) || comp_data(data, parque->current) || comp_data(data, veiculo->toplog->entrada) || eh29deFev(data)) return 4;
    
    return 0;
}


void remove_veiculo(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]) {
    
    if (sys == NULL || nomeParque == NULL || matricula == NULL) {
        printf("Invalid parameters.\n");
        return;
    }

    switch (valida_saida(sys, nomeParque, matricula, data)) {
        case 0:
            
            Veiculo *veiculo = procura_veiculo(sys, matricula);
            if (veiculo == NULL) {
                printf("Veiculo nao encontrado.\n");
                return;
            }

            Parque *parque = procuraParque(sys, nomeParque);
            if (parque == NULL) {
                printf("Parque nao encontrado.\n");
                return;
            }
            float valor = calcula_tarifa(parque, veiculo->toplog->entrada, data);
            

            atualiza_log_saida(parque, veiculo, data, valor);
            
            parque->lugares_livres++;
            veiculo->inside = 0;
            strcpy(parque->current, data);
            

            char *dia = dataP(data);
            insere_faturacao(parque, dia, valor);

            printf("%s %s %s %.2f\n", matricula, veiculo->toplog->entrada, data, valor);
            free(dia);
            break;
        case 1:
            printf("%s: no such parking.\n", nomeParque);
            break;
        case 2:
            printf("%s: invalid licence plate.\n", matricula);
            break;
        case 3:
            printf("%s: invalid vehicle exit.\n", matricula);
            break;
        case 4:
            printf("invalid date.\n");
            break;

        default:    
            break;
    }
}

void lista_faturacao(Sistema *sys, char *nomeParque, char *data) {
    Parque *parque = procuraParque(sys, nomeParque);
    if (parque == NULL) {
        printf("%s: no such parking.\n", nomeParque);
        return;
    }

    Faturacao *faturacao = parque->faturacao;
    Log *logp = parque->headlog;
    char horaS[6] = ""; 
    
    if (data[0] == '\0') {
        char *diaS = NULL; 
        while (faturacao != NULL) {
            diaS = dataP(faturacao->dia);
            if (diaS != NULL) {
                printf("%s %.2f\n", diaS, faturacao->valor);
                free(diaS); 
            }
            faturacao = faturacao->next;
        }
        return;
    }

    while (logp != NULL) {
        if (strncmp(data, logp->saida, 10) == 0) {
            strcpy(horaS, horaP(logp->saida));
            printf("%s %s %.2f\n", logp->matricula, horaS, logp->valor_p);
        }
        logp = logp->nextp;
    }
}