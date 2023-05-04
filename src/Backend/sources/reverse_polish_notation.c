#include "../headers/reverse_polish_notation.h"

/// @brief | Преобразует входную строку в обратную польскую нотацию |
/// @param  input_array - входная строка char *
/// @param  amount_tokens - указатель на int
/// @return массив лексем
token *reverse_polish_notation(char *input_array, int *amount_tokens) {
  token *reverse_polish_notation_array = NULL;
  if (input_array != NULL && amount_tokens != NULL) {
    *amount_tokens = 0;
    token *token_array = get_token_array(input_array, amount_tokens);
    if (token_array != NULL) {
      int result = check_token_array(token_array, *amount_tokens);
      if (result == OK) {
        reverse_polish_notation_array =
            get_reverse_polish_notation_array(token_array, amount_tokens);
      }
      free(token_array);
      token_array = NULL;
    }
  }
  return reverse_polish_notation_array;
}

/// @brief | Преобразует входную строку в массив лексем |
/// WARNING! Память выделяется динамически, необходима очистка
/// @param  input_array - входная строка char *
/// @return
/// массив лексем token *
token *get_token_array(char *input_array, int *amount_tokens) {
  token *token_array = NULL;
  if (input_array != NULL && amount_tokens != NULL) {
    *amount_tokens = 0;
    char *pointer = input_array;
    double value = 0.0;
    int size_token_array = 5;
    token_array = init_token_array(size_token_array);
    char *name_operator = init_char_array(SIZE_NAME_FUNC);
    int status = -1;
    while (*pointer != '\0') {
      if (*amount_tokens == size_token_array - 1) {
        token_array = realloc_token_array(token_array, &size_token_array);
      }
      if (isdigit(*pointer)) {
        value = get_value(&pointer);
        init_token(&token_array[*amount_tokens], e_simple_number_status, value,
                   name_operator);
        ++(*amount_tokens);
      } else if (isalpha(*pointer)) {
        get_name_operators(&pointer, name_operator);
        status = get_status(name_operator[0]);
        init_token(&token_array[*amount_tokens], status, value, name_operator);
        ++(*amount_tokens);
      } else {
        name_operator[0] = *pointer;
        ++pointer;
        init_token(&token_array[*amount_tokens], e_operator_status, value,
                   name_operator);
        ++(*amount_tokens);
      }
      value = 0.0;
      status = -1;
      memset(name_operator, '\0', SIZE_NAME_FUNC);
    }
    if (!(*amount_tokens)) {
      free(token_array);
      token_array = NULL;
    }
    free(name_operator);
    name_operator = NULL;
  }
  return token_array;
}

/// @brief | Создает указатель на массив лексем |
/// @param amount_tokens - размер массива const int
/// @return
/// указатель на  массив лексем
token *init_token_array(const int amount_tokens) {
  return (token *)calloc(amount_tokens, sizeof(token));
}

/// @brief | Изменяет размер массива лексем |
/// @param  token_array - массив лексем token *
/// @param prev_size - указатель на исходный размер массива int *
/// @return
/// измененный массив лексем
token *realloc_token_array(token *token_array, int *prev_size) {
  *prev_size *= 2;
  token_array = (token *)realloc(token_array, *prev_size * sizeof(token));
  return token_array;
}

/// @brief | Получает вещественное число из входной строки |
/// @param  pointer - указатель на входную строку char *
/// @return
/// число double
double get_value(char **pointer) {
  double value = 0.0;
  if (pointer != NULL) {
    int start_length = 5;
    char *number = init_char_array(start_length);
    int i = 0;
    while (isdigit(**pointer) || **pointer == '.') {
      if (i == start_length - 1) {
        number = realloc_char_array(number, &start_length);
      }
      number[i++] = **pointer;
      ++(*pointer);
    }
    number[i] = '\0';
    setlocale(LC_NUMERIC, "C");
    value = strtod(number, NULL);
    free(number);
    number = NULL;
  }
  return value;
}

/// @brief | Получает имена операторов(функций) из входной строки |
/// @param  pointer - указатель на входную строку char *
/// @param name_operator - указатель на строку char *
void get_name_operators(char **pointer, char *name_operator) {
  if (pointer != NULL && name_operator != NULL) {
    if (**pointer == 'm' || **pointer == 'c' || **pointer == 't') {
      name_operator[0] = **pointer;
      *pointer += 3;
    } else if (**pointer == 'p') {
      name_operator[0] = **pointer;
      *pointer += 2;
    } else if (**pointer == 's') {
      strncpy(name_operator, *pointer, 2);
      ++(*pointer);
      if (**pointer == 'i') {
        *pointer += 2;
      } else {
        *pointer += 3;
      }
    } else if (**pointer == 'a') {
      strncpy(name_operator, *pointer, 2);
      *pointer += 4;
    } else if (**pointer == 'l') {
      name_operator[0] = **pointer;
      ++(*pointer);
      if (**pointer == 'n') {
        name_operator[1] = **pointer;
        ++(*pointer);
      } else {
        name_operator[1] = 'g';
        *pointer += 2;
      }
    } else {
      name_operator[0] = **pointer;
      ++(*pointer);
    }
  }
}

/// @brief | Возвращает статус лексемы |
/// @param  symbol - первый символ оператора(функции)
/// @return статус лексемы int
int get_status(char symbol) {
  int status = -1;
  if (symbol == 'p' || symbol == 'x' || symbol == 'e') {
    status = e_special_number_status;
  } else if (symbol == 'm') {
    status = e_operator_status;
  } else {
    status = e_function_status;
  }
  return status;
}

/// @brief | Преобразование массива лексем в обратную польскую нотацию |
/// WARNING! Память выделяется динамически, необходима очистка
/// @param token_array - массив лексем token *
/// @param amount_tokens - количество лексем
/// @return преобразованная строка лексем
token *get_reverse_polish_notation_array(token *token_array,
                                         int *amount_tokens) {
  token *reverse_polish_notation_array = NULL;
  if (token_array != NULL) {
    reverse_polish_notation_array = init_token_array(*amount_tokens);
    stack_tokens *stack_operators = init_stack();
    int count = 0;
    for (int i = 0; i < *amount_tokens; ++i) {
      if (token_array[i].status == e_simple_number_status ||
          token_array[i].status == e_special_number_status) {
        reverse_polish_notation_array[count++] = token_array[i];
      } else if (token_array[i].status == e_function_status) {
        stack_operators = push_token(stack_operators, token_array[i]);
      } else {
        if (token_array[i].priority == e_right_bracket_priority) {
          stack_operators = operator_is_right_bracket(
              reverse_polish_notation_array, stack_operators, &count);
          if (stack_operators == NULL) {
            break;
          }
        } else if (token_array[i].priority == e_left_bracket_priority ||
                   stack_operators->size == EMPTY ||
                   token_array[i].priority >
                       stack_operators->token_object.priority) {
          stack_operators = push_token(stack_operators, token_array[i]);
        } else if (token_array[i].priority <=
                   stack_operators->token_object.priority) {
          stack_operators =
              priority_is_less(token_array[i], reverse_polish_notation_array,
                               stack_operators, &count);
        }
      }
    }
    cleaning_stack_operators(reverse_polish_notation_array, stack_operators,
                             &count);
    *amount_tokens = count;
  }
  return reverse_polish_notation_array;
}

/// @brief | Обр.польская нотация: лексема - правая скобка |
/// @param reverse_polish_notation_array - преобразованный массив лексем token *
/// @param stack_operators - стек операторов stack_tokens *
/// @param count - указатель на количество элементов в массиве лексем
/// @return указатель на стек операторов
stack_tokens *operator_is_right_bracket(token *reverse_polish_notation_array,
                                        stack_tokens *stack_operators,
                                        int *count) {
  if (reverse_polish_notation_array != NULL && stack_operators != NULL &&
      count != NULL) {
    while (stack_operators->size != EMPTY &&
           stack_operators->token_object.priority != e_left_bracket_priority) {
      reverse_polish_notation_array[(*count)++] = pop_token(&stack_operators);
    }
    if (stack_operators->token_object.priority == e_left_bracket_priority) {
      pop_token(&stack_operators);
    } else {
      free_stack(stack_operators);
      free(reverse_polish_notation_array);
      reverse_polish_notation_array = NULL;
      *count = 0;
    }
    if (stack_operators->token_object.status == e_function_status) {
      reverse_polish_notation_array[(*count)++] = pop_token(&stack_operators);
    }
  }
  return stack_operators;
}

/// @brief | Обр.польская нотация : приоритет оператора меньше, чем
/// у верхнего оператора в стеке |
/// @param token_object - проверяемая лексема
/// @param reverse_polish_notation_array - преобразованный массив лексем token *
/// @param stack_operators - стек операторов stack_tokens *
/// @param count - указатель на количество элементов в массиве лексем
///@return указатель на стек операторов
stack_tokens *priority_is_less(const token token_object,
                               token *reverse_polish_notation_array,
                               stack_tokens *stack_operators, int *count) {
  if (reverse_polish_notation_array != NULL && stack_operators != NULL &&
      count != NULL) {
    while (stack_operators->size != EMPTY &&
           token_object.priority <= stack_operators->token_object.priority) {
      reverse_polish_notation_array[(*count)++] = pop_token(&stack_operators);
    }
    stack_operators = push_token(stack_operators, token_object);
  }
  return stack_operators;
}

/// @brief | Обр.польская нотация: очистка стека операторов после цикла |
/// @param reverse_polish_notation_array - преобразованный массив лексем token *
/// @param stack_operators - стек операторов stack_tokens *
/// @param count - указатель на количество элементов в массиве лексем
void cleaning_stack_operators(token *reverse_polish_notation_array,
                              stack_tokens *stack_operators, int *count) {
  if (reverse_polish_notation_array != NULL && stack_operators != NULL &&
      count != NULL) {
    while (stack_operators->size != EMPTY) {
      reverse_polish_notation_array[(*count)++] = pop_token(&stack_operators);
    }
    free_stack(stack_operators);
  }
}

/// @brief | Дополнительная обработка ошибок массива лексем |
/// @param token_array - массив лексем token *
/// @param amount_tokens - количество лексем int
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int check_token_array(token *token_array, const int amount_tokens) {
  int output = OK;
  if (token_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    int (*functions[NUM_CHECK_FUNCTIONS])(token *,
                                          const int) = {NAMES_FUNCTIONS};
    for (int i = 0; i < NUM_CHECK_FUNCTIONS && output == OK; ++i) {
      output = functions[i](token_array, amount_tokens);
    }
  }
  return output;
}

/// @brief | Находит унарные плюсы и минусы |
/// @param token_array - массив лексем token *
/// @param amount_tokens - количество лексем int
/// @return
/// 0 - OK;
/// 2 - ошибка памяти
int find_unar_operators(token *token_array, const int amount_tokens) {
  int output = OK;
  if (token_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    for (int i = 0; i < amount_tokens; ++i) {
      if (token_array[i].status == e_operator_status &&
          token_array[i].priority == e_sum_priority) {
        if ((i == 0 ||
             (i != 0 && token_array[i - 1].status == e_operator_status &&
              token_array[i - 1].priority == e_left_bracket_priority)) &&
            i != amount_tokens - 1 &&
            (token_array[i + 1].status == e_function_status ||
             token_array[i + 1].status == e_simple_number_status ||
             token_array[i + 1].status == e_special_number_status)) {
          if (token_array[i].name[0] == '+') {
            token_array[i].priority = e_unar_sum_priority;
          } else {
            token_array[i].priority = e_unar_sub_priority;
          }
        }
      }
    }
  }
  return output;
}

/// @brief | Определяет пустые скобки вида () |
/// @param token_array - массив лексем token *
/// @param amount_tokens - количество лексем int
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int find_empty_brackets(token *token_array, const int amount_tokens) {
  int output = OK;
  if (token_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    for (int i = 0; i < amount_tokens && output == OK; ++i) {
      if (token_array[i].status == e_operator_status &&
          token_array[i].priority == e_left_bracket_priority &&
          token_array[i + 1].status == e_operator_status &&
          token_array[i + 1].priority == e_right_bracket_priority) {
        output = SYNTAX_ERROR;
      }
    }
  }
  return output;
}

/// @brief | Определяет есть ли между функциями операторы |
/// ошибки вида: sin(x)cos(x)
/// @param token_array - массив лексем token *
/// @param amount_tokens - количество лексем int
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int check_functions(token *token_array, const int amount_tokens) {
  int output = OK;
  if (token_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    for (int i = 0; i < amount_tokens && output == OK; ++i) {
      if (token_array[i].status == e_function_status && i != 0 &&
          token_array[i - 1].status == e_operator_status &&
          (token_array[i - 1].priority == e_right_bracket_priority ||
           token_array[i - 1].priority == e_mod_priority)) {
        output = SYNTAX_ERROR;
      }
    }
  }
  return output;
}
