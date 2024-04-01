/* iaed24 - ist1105865 - project */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "quicksort.h"
#include "main.h"


/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */
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

/* Le um nome para a string que recebe como parametro. */
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
    primeiros_10[10] = '\0'; // Atribui o caractere nulo na posição 10
    return primeiros_10;
}

//////////////////////////////////////////////////////////////////////////


int hash_string(char matricula[SIZMAT], int tamanho_hashmap) {
    int hash = 0;
    for (int i = 0; matricula[i] != '\0'; i++) {
        hash = (hash * 31 + matricula[i]) % tamanho_hashmap; // Escolha de um número primo para a função de hash
    }
    return hash;
}


// Função para inserir um veículo no hash map do sistema
void insere_veiculo(Sistema *sys, Veiculo *veiculo) {
    // Calcula o índice do hash baseado na matrícula do veículo;
    int indice = hash_string(veiculo->matricula, sys->hashmap_size);

    // Verifica se há um veículo já inserido neste índice
    if (sys->Vhashmap[indice] == NULL) {
        // Não há colisão, então simplesmente insere o veículo neste índice
        sys->Vhashmap[indice] = veiculo;
        veiculo->next = NULL;
    } else {
        // Há colisão, então usamos encadeamento separado
        // Insere o veículo no final da lista encadeada neste índice

        Veiculo *atual = sys->Vhashmap[indice]; // Guarda o início da lista
        while(atual->next != NULL) {
            atual = atual->next; // Avança até o final da lista
        }
        atual->next = veiculo; // Adiciona o novo veículo no final
        veiculo->next = NULL; // Garante que o novo veículo seja o último da lista
    }
}


// Função para procurar um veículo no hash map do sistema
Veiculo *procura_veiculo(Sistema *sys, char *matricula) {
    // Calcula o índice do hash baseado na matrícula do veículo
    int indice = hash_string(matricula, sys->hashmap_size);

    // Verifica se o veículo está no índice calculado
    Veiculo *veiculo = sys->Vhashmap[indice];
    if (veiculo == NULL) {
        // Veículo não encontrado neste índice
        return NULL;
    }

    // Verifica se a matrícula corresponde ao primeiro veículo na lista encadeada
    if (strcmp(veiculo->matricula, matricula) == 0) {
        return veiculo; // Matrícula encontrada
    } else {
        // Pode haver colisão, então percorremos a lista encadeada neste índice
        // e procuramos o veículo com a matrícula correspondente
        while (veiculo->next != NULL) {
            veiculo = veiculo->next; // Próximo veículo na lista
            if (strcmp(veiculo->matricula, matricula) == 0) {
                return veiculo; // Matrícula encontrada
            }
        }
    }

    // Matrícula não encontrada
    return NULL;
}



Log *inicializa_log(Veiculo *veiculo, Parque *parque, char *entrada) {
    if (veiculo == NULL || parque == NULL || entrada == NULL) {
        return NULL; // Verifica se os ponteiros são válidos
    }

    // Aloca memória para o novo log
    Log *novo_log = (Log *)malloc(sizeof(Log));
    if (novo_log == NULL) {
        printf("Erro ao alocar memória para o log.\n");
        return NULL;
    }
    
    // Aloca memória e copia a matrícula do veículo
    novo_log->matricula = (char *)malloc((strlen(veiculo->matricula) + 1) * sizeof(char));
    if (novo_log->matricula == NULL) {
        printf("Erro ao alocar memória para a matrícula do veículo no log.\n");
        free(novo_log);
        return NULL;
    }
    strcpy(novo_log->matricula, veiculo->matricula);
    
    // Aloca memória e copia o nome do parque
    novo_log->nome_p = (char *)malloc((strlen(parque->nome) + 1) * sizeof(char));
    if (novo_log->nome_p == NULL) {
        printf("Erro ao alocar memória para o nome do parque no log.\n");
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
    if (veiculo == NULL || saida == NULL) {
        return;
    }

    // Verifica se o veículo possui pelo menos um log
    if (veiculo->toplog == NULL) {
        return;
    }

    // Atualiza a data de saída e o valor pago no log do topo
    strcpy(veiculo->toplog->saida, saida);
    veiculo->toplog->valor_p = valor_pago;

    // Adiciona o log atualizado à lista de logs do parque

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
        printf("Erro: Parque não encontrado para adicionar o log.\n");
    }
}

void remove_log_veiculo(Veiculo *veiculo, Log *log) {
    if (veiculo == NULL || log == NULL) {
        printf("Erro: Veículo ou log inválido.\n");
        return;
    }

    // Verifica se a pilha de logs do veículo está vazia
    if (veiculo->toplog == NULL) {
        printf("Erro: Pilha de logs do veículo vazia.\n");
        return;
    }

    // Caso especial: se o log a ser removido for o topo da pilha
    if (veiculo->toplog == log) {
        veiculo->toplog = log->prevV; // Atualiza o topo da pilha
        free(log->matricula); // Libera a memória alocada para a matrícula
        free(log); // Libera a memória alocada para o log
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
        free(log->matricula); // Libera a memória alocada para a matrícula
        free(log); // Libera a memória alocada para o log
    } else {
        printf("Erro: Log não encontrado no veículo.\n");
    }
}


void remove_todos_logs_parque(Sistema *sys, Parque *parque) {
    if (parque == NULL) {
        printf("Erro: Parque inválido.\n");
        return;
    }

    // Percorre todos os logs do parque
    Log *log = parque->headlog;
    while (log != NULL) {
        if (log->nome_p != NULL) {
            // Sinaliza o log a ser removido
            log->nome_p = NULL;
            // Procura o veículo correspondente e remove o log da pilha
            Veiculo *veiculo = procura_veiculo(sys, log->matricula);
            if (veiculo != NULL) {
                
                if (veiculo->inside == 1 && strcmp(parque->nome, veiculo->toplog->nome_p) == 0) {
                    // Remover o primeiro log da pilha do veículo
                    Log *temp = veiculo->toplog;
                    veiculo->toplog = veiculo->toplog->prevV;
                    free(temp->matricula);
                    free(temp);
                    veiculo->inside = 0; // Atualiza veiculo->inside para 0
                }
                remove_log_veiculo(veiculo, log);
            }
        }
        log = log->nextp;
    }
}





void listar_logs_veiculo(Veiculo *veiculo) {
    if (veiculo == NULL) {
        return;
    }

    // Ponteiro para percorrer a pilha de logs do veículo
    Log *temp_log = veiculo->toplog;

    // Array de ponteiros para os logs do veículo (inicialmente vazio)
    Log **logs = NULL;
    int num_logs = 0; // Número de logs do veículo

    // Armazenar os logs do veículo em um array dinâmico
    while (temp_log != NULL) {
        logs = realloc(logs, (num_logs + 1) * sizeof(Log *));
        if (logs == NULL) {
            printf("Erro de alocação de memória.\n");
            return;
        }
        logs[num_logs++] = temp_log;
        temp_log = temp_log->prevV;
    }

    // Ordenação dos logs do veículo (usando quicksort personalizado)
    quicksort_logs(logs, 0, num_logs - 1);

    // Imprimir os logs do veículo
    for (int i = 0; i < num_logs; i++) {
        printf("%s %s", logs[i]->nome_p, logs[i]->entrada);
        if (logs[i]->saida[0] != '\0') {
            printf(" %s", logs[i]->saida);
        }
        printf("\n");
    }

    // Liberar a memória alocada para o array de ponteiros para os logs
    free(logs);
}


/////////////////////////////////////////////////////////////////////////

Parque *procuraParque(Sistema *sys, char *nome) {
    Parque *head = sys->headparque;
    while(head != NULL) {
    if(head->nome != NULL && strcmp(head->nome, nome) == 0) {
        return head;
    }
    head = head->next;
}
    return NULL;
}


///@brief Função que valida um parque
///@param nome Nome do parque
///@param cap Capacidade do parque
///@param X Valor X
///@param Y Valor Y
///@param Z Valor Z
///@return 0 se for válido, 1 se o sistema nao admitir mais parques, 2 se a capacidade for 
///        inválida, 3 se os valores de X, Y ou Z forem inválidos e 4 se o parque já existir
int valida_parque(Sistema *sys, char *nome, int cap, double X, double Y, double Z) {
    if(sys->num_parques >= MAXPARQUES) return 1;
    if(procuraParque(sys, nome) != NULL) return 2;
    if(cap <= 0) return 3;
    if ( X > Y || Y > Z || X <= 0 || Y <= 0 || Z <= 0) return 4;
    return 0;
}


///@brief Função que cria um parque
///@param sys Sistema
///@param nome Nome do parque
///@param cap Capacidade do parque
///@param X Valor X
///@param Y Valor Y
///@param Z Valor Z
///@return void cria um parque no sistema
void cria_parque(Sistema *sys, char *nome, int cap, double X, double Y, double Z) {
    Parque *parque = NULL;
    parque = (Parque *)malloc(sizeof(Parque));
    if (parque == NULL) {
        printf("Erro ao alocar memória para o parque.\n");
        free(parque);
        return;
    }
    switch (valida_parque(sys, nome, cap, X, Y, Z))
    {
    case 0:

        parque->nome = strdup(nome);
        if(parque->nome == NULL) {
            printf("Erro ao alocar memória para o nome do parque.\n");
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

///@brief Função que lista os parques
///@param sys Sistema 
///@return void lista os parques por ordem de criação
void lista_parques(Sistema *sys) {
    Parque *current = sys->headparque;
    while (current != NULL) {
        printf("%s %d %d\n", current->nome, current->capacidade, current->lugares_livres);
        current = current->next;
    }
    return;
}

void imprime_parques_alfa(Sistema *sys) {

    // Vetor para armazenar os nomes dos parques
    char *nomes[MAXPARQUES];
    int num_parques = 0;

    // Preenche o vetor com os nomes dos parques
    Parque *atual = sys->headparque;
    while (atual != NULL && num_parques < MAXPARQUES) {
        nomes[num_parques] = atual->nome;
        atual = atual->next;
        num_parques++;
    }

    // Ordena o vetor de nomes dos parques por ordem alfabética
    for (int i = 0; i < num_parques - 1; i++) {
        for (int j = i + 1; j < num_parques; j++) {
            if (strcmp(nomes[i], nomes[j]) > 0) {
                char *temp = nomes[i];
                nomes[i] = nomes[j];
                nomes[j] = temp;
            }
        }
    }

    // Imprime os nomes dos parques na ordem correta
    for (int i = 0; i < num_parques; i++) {
        printf("%s\n", nomes[i]);
    }
}


void remove_parque(Sistema *sys, char *nome_parque) {
    Parque *parque = NULL;
    parque = procuraParque(sys, nome_parque);
    // Se o parque não for encontrado, imprimir mensagem de erro
    if (parque == NULL) {
        printf("%s: no such parking.\n", nome_parque);
        return;
    }

    // Remover o parque da lista de parques
    
    if (parque->prev != NULL)
        parque->prev->next = parque->next;
    if (parque->next != NULL)
        parque->next->prev = parque->prev;

    if (sys->headparque == parque)
        sys->headparque = parque->next; // Se for o primeiro parque da lista
    sys->num_parques--;
    
    remove_todos_logs_parque(sys, parque);
    
    while(parque->faturacao != NULL) {
        Faturacao *temp = parque->faturacao;
        parque->faturacao = parque->faturacao->next;
        free(temp);
    }
    free(parque->nome); // Liberar a memória alocada para o nome do parque
    free(parque); // Liberar a memória alocada para o parque
    imprime_parques_alfa(sys);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////


void leData(char data[MAXDATA]) {
    int i = 0, c;

    while ((c = getchar()) != '\t' && c != '\n') {
        data[i++] = c;
    }
    data[i] = '\0';

    if (strlen(data) == 15) {    
        // Adiciona um zero ao minuto se a hora for menor que 10
        for (int i = strlen(data); i >= 11; i--) {
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


int valida_entrada(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]) {
    Parque *parque = NULL;  
    parque = procuraParque(sys, nomeParque);
    if (parque == NULL) return 1;
    if (parque->lugares_livres <= 0) return 2;
    if (valida_matricula(matricula)) return 3;
    Veiculo *veiculo = NULL;
    veiculo = procura_veiculo(sys, matricula);
    if (veiculo != NULL && veiculo->inside == 1) return 4;
    if (valida_data(data) || comp_data (data, parque->current)|| eh29deFev(data)) return 5;
    
    return 0;
}

// Função auxiliar para adicionar um log ao veículo
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
            veiculo->toplog = NULL; // Inicialize a pilha de logs do veículo como NULL
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
            printf("%s %d\n", nomeParque, parque->lugares_livres);
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

// Função para calcular a quantidade total de minutos até uma data
int calcular_minutos_total(int dia, int mes, int ano, int hora, int minuto) {
    int minutos = 0;

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

// Função para calcular a quantidade de minutos passados de uma data para outra
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

void insere_faturacao(Parque *parque, char *dia, float valor) {
    // If the faturacao list is empty, create the first element
    if (parque != NULL && parque->faturacao == NULL) {
        parque->faturacao = (Faturacao *)malloc(sizeof(Faturacao));
        if (parque->faturacao == NULL) {
            printf("Erro ao alocar memória para a faturação.\n");
            return;
        }

        strcpy(parque->faturacao->dia, dia);
        parque->faturacao->valor = valor;
        parque->faturacao->next = NULL;
    } else {
        // Traverse the list to find the node with the specified date
        Faturacao *current = parque->faturacao;
        while (current != NULL && current->next != NULL && strcmp(current->dia, dia) != 0) {
            current = current->next;
        }

        if (current != NULL && strcmp(current->dia, dia) == 0) {
            // If the node with the specified date exists, increment the value
            current->valor += valor;
        } else {
            // If the node with the specified date doesn't exist, create a new node
            if (current != NULL) {
                current->next = (Faturacao *)malloc(sizeof(Faturacao));
                if (current->next == NULL) {
                    printf("Erro ao alocar memória para a faturação.\n");
                    return;
                }

                current = current->next;
                strcpy(current->dia, dia);
                current->valor = valor;
                current->next = NULL;
            } else {
                printf("Erro: Lista de faturação não inicializada corretamente.\n");
            }
        }
    }
}



void remove_veiculo(Sistema *sys, char *nomeParque, char matricula[SIZMAT], char data[MAXDATA]) {
    switch (valida_saida(sys, nomeParque, matricula, data)) {
        case 0:
            // Check if sys, nomeParque, and matricula are valid pointers
            if (sys == NULL || nomeParque == NULL || matricula == NULL) {
                printf("Invalid parameters.\n");
                return;
            }
            
            Veiculo *veiculo = procura_veiculo(sys, matricula);
            if (veiculo == NULL) {
                printf("Veículo não encontrado.\n");
                return;
            }

            Parque *parque = procuraParque(sys, nomeParque);
            if (parque == NULL) {
                printf("Parque não encontrado.\n");
                return;
            }
            float valor = calcula_tarifa(parque, veiculo->toplog->entrada, data);
            
            // Atualiza os campos do log do veículo
            atualiza_log_saida(parque, veiculo, data, valor);
            
            parque->lugares_livres++;
            veiculo->inside = 0;
            strcpy(parque->current, data);
            
            // Adiciona a faturação ao parque
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

int count_logs(Log *log) {
    int count = 0;
    while (log != NULL) {
        count++;
        log = log->nextp;
    }
    return count;
}

void lista_faturacao(Sistema *sys, char *nomeParque, char *data) {
    Parque *parque = procuraParque(sys, nomeParque);
    if (parque == NULL) {
        printf("%s: no such parking.\n", nomeParque);
        return;
    }

    Faturacao *faturacao = parque->faturacao;
    Log *logp = parque->headlog;
    char horaS[6] = ""; // Make sure to initialize the array
    
    if (data[0] == '\0') {
        char *diaS = NULL; // Declare diaS pointer here
        while (faturacao != NULL) {
            diaS = dataP(faturacao->dia);
            if (diaS != NULL) {
                printf("%s %.2f\n", diaS, faturacao->valor);
                free(diaS); // Free diaS after use
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


 
//////////////////////////////////////////////////////////////////////////////////

/// @brief Função que executa o comando Q
/// @return void
void in_hand_q() {
    exit(0);
    return;
}

/// @brief Função que executa o comando P
/// @param sys Sistema
/// @return void
void in_hand_p(Sistema *sys) {
    char *nome = NULL; 
    int cap;
    double X, Y, Z;
    int fimLinha = leEspacos();

    if (!fimLinha){ 
        lista_parques(sys); 
        return;
    }

    // Alocar memória dinamicamente para armazenar o nome do parque
    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); // Passando nome como argumento
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

    // Libera a memória alocada para o nome após o uso
    free(nome);
    return;
}

/// @brief Função que executa o comando E
/// @param sys Sistema
/// @return void
void in_hand_e(Sistema *sys) {
    char *nome = NULL;
    char matricula[SIZMAT];
    char data[MAXDATA] = "";
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }

    // Alocar memória dinamicamente para armazenar o nome do parque
    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); // Passando nome como argumento
    leMatricula(matricula);
    leData(data);
    adiciona_veiculo(sys, nome, matricula, data);
    free(nome);
    return;
}   

/// @brief Função que executa o comando S
/// @param sys Sistema
/// @return void
void in_hand_s(Sistema *sys) {
    char *nome = NULL;
    char matricula[SIZMAT];
    char data[MAXDATA] = "";
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }

    // Alocar memória dinamicamente para armazenar o nome do parque
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

/// @brief Função que executa o comando V
/// @param sys Sistema
/// @return void
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

/// @brief Função que executa o comando F
/// @param sys Sistema
/// @return void
void in_hand_f(Sistema *sys) {
    char *nome = NULL;
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }

    // Alocar memória dinamicamente para armazenar o nome do parque
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


/// @brief Função que executa o comando R
/// @param sys Sistema
/// @return void
void in_hand_r(Sistema *sys) {
    char *nome = NULL;
    int fimLinha = leEspacos();
    if(!fimLinha) {
        return;
    }

    // Alocar memória dinamicamente para armazenar o nome do parque
    nome = (char *)malloc(BUFFSIZ * sizeof(char));
    if (nome == NULL) {
        printf("Erro ao alocar memória para o nome do parque.\n");
        return;
    }

    leNome(&nome); // Passando nome como argumento
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

////////////////////////////////////////////////////////////////////////////////////


Sistema *inicializa_sistema(int tamanho_hashmap) {
    Sistema *sys = (Sistema *)malloc(sizeof(Sistema));
    if (sys == NULL) {
        printf("Erro ao alocar memória para o sistema.\n");
        exit(1);
    }
    sys->headparque = NULL;
    sys->num_parques = 0;
    sys->hashmap_size = tamanho_hashmap;
    sys->Vhashmap = (Veiculo **)malloc(tamanho_hashmap * sizeof(Veiculo *));
    if (sys->Vhashmap == NULL) {
        printf("Erro ao alocar memória para o hash map de veículos.\n");
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

int main() {
    Sistema *sys = inicializa_sistema(47837);
    while(1) {
        input_handler(sys);
    }
    free_sistema(sys);
    return 0; 
}

