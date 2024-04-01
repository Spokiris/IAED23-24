/* iaed24 - ist1105865 - input.h */
#ifndef INPUT_H
#define INPUT_H

#include "quicksort.h"
#include "parser.h"
#include "input.h"
#include "system.h"

/// @brief Função que executa o comando Q
/// @return void
void in_hand_q();

/// @brief Função que executa o comando P
/// @param sys Sistema
/// @return void
void in_hand_p(Sistema *sys);

/// @brief Função que executa o comando E
/// @param sys Sistema
/// @return void
void in_hand_e(Sistema *sys);

/// @brief Função que executa o comando S
/// @param sys Sistema
/// @return void
void in_hand_s(Sistema *sys);

/// @brief Função que executa o comando V
/// @param sys Sistema
/// @return void
void in_hand_v(Sistema *sys);

/// @brief Função que executa o comando F
/// @param sys Sistema
/// @return void
void in_hand_f(Sistema *sys);

/// @brief Função que executa o comando R
/// @param sys Sistema
/// @return void
void in_hand_r(Sistema *sys);

/// @brief  Função que lê o input e chama a função correspondente
/// @param sys  Sistema
void input_handler(Sistema *sys);

#endif // INPUT_H