#include "../headers/validation.h"

/// @brief | Проверка входной строки на корректный ввод |
/// @param  input_array - входная строка char *
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int input_array_validation(char *input_array) {
  int output = OK;
  if (input_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    int (*functions[AMOUNT_CHECK_FUNCTIONS])(char *) = {NAMES_CHECK_FUNCTIONS};
    for (int i = 0; i < AMOUNT_CHECK_FUNCTIONS && output == OK; i++) {
      output = functions[i](input_array);
    }
  }
  return output;
}

/// @brief | Проверка на некорректные символы |
/// @param  input_array - входная строка char *
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int check_allowed_characters(char *input_array) {
  int output = OK;
  if (input_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    size_t length = strlen(input_array);
    for (size_t i = 0; i < length && output == OK; i++) {
      char character = input_array[i];
      if (character < 40 || character == 44 ||
          (character > 57 && character < 94) ||
          (character > 94 && character < 97) || character > 122) {
        output = SYNTAX_ERROR;
      }
    }
  }
  return output;
}

/// @brief | Проверка количества скобок |
/// @param  input_array - входная строка char *
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int check_amount_brackets(char *input_array) {
  int output = OK;
  if (input_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    size_t length = strlen(input_array);
    int count_brackets = 0;
    for (size_t i = 0; i < length && count_brackets >= 0; i++) {
      if (input_array[i] == '(') {
        count_brackets++;
      } else if (input_array[i] == ')') {
        count_brackets--;
      }
    }
    if (count_brackets != 0) {
      output = SYNTAX_ERROR;
    }
  }
  return output;
}

/// @brief | Проверка названий операторов (функций) |
/// @param  input_array - входная строка char *
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int check_name_operators(char *input_array) {
  int output = OK;
  if (input_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    size_t length = strlen(input_array);
    char *start = NULL;
    char *end = NULL;
    for (size_t i = 0; i < length && output == OK; i++) {
      if (isalpha(input_array[i]) && start == NULL) {
        start = &input_array[i];
        end = &input_array[i];
      } else if (isalpha(input_array[i]) && start != NULL) {
        end = &input_array[i];
      }
      if ((!(isalpha(input_array[i])) || input_array[i + 1] == '\0') &&
          start != NULL) {
        size_t size = end - start + 1;
        if (start[0] == 'a' || start[0] == 'c' || start[0] == 's' ||
            start[0] == 't' || start[0] == 'l') {
          size += 1;
        }
        char *potential_operator = init_char_array(size + 1);
        if (potential_operator == NULL) {
          output = MEMORY_ERROR;
        } else {
          potential_operator = strncpy(potential_operator, start, size);
          output = compare_name_operators(potential_operator);
          free(potential_operator);
          potential_operator = NULL;
          start = NULL;
          end = NULL;
        }
      }
    }
  }
  return output;
}

/// @brief | Сравнивает название оператора во входной строке с оригинальным
/// названием |
/// @param  input_array - входная строка char *
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int compare_name_operators(char *potential_operator) {
  int output = OK;
  if (potential_operator == NULL) {
    output = MEMORY_ERROR;
  } else {
    int match = 1;
    const char names_operators[AMOUNT_OPERATORS][10] = NAMES_OPERATORS;
    for (int i = 0; i < AMOUNT_OPERATORS && match; i++) {
      match = strcmp(potential_operator, names_operators[i]);
    }
    if (match) {
      output = SYNTAX_ERROR;
    }
  }
  return output;
}

/// @brief | Проверяет корректность ввода точек вещественных чисел |
/// @param  input_array - входная строка char *
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
/// 2 - ошибка памяти
int check_points(char *input_array) {
  int output = OK;
  if (input_array == NULL) {
    output = MEMORY_ERROR;
  } else {
    size_t length = strlen(input_array);
    char *start = NULL;
    char *end = NULL;
    char *point = NULL;
    for (size_t i = 0; i < length && output == OK; i++) {
      output = find_point(&input_array[i], &start, &point, &end);
      if (output == OK) {
        output = check_pointers(&input_array[i], &start, &point, &end);
      }
      if (output == SYNTAX_ERROR) {
        start = NULL;
        end = NULL;
        point = NULL;
      }
    }
  }
  return output;
}

/// @brief | Расстановка указателей на первую цифру, точку, последнюю цифру |
/// @param  symbol - указатель на символ char *
/// @param  start - указатель на начало
/// @param  point - указатель на точку
/// @param  end - указатель на конец
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
int find_point(char *symbol, char **start, char **point, char **end) {
  int output = OK;
  if (isdigit(*symbol) && *start == NULL) {
    *start = symbol;
  } else if (isdigit(*symbol) && *start != NULL) {
    *end = symbol;
  } else if (*symbol == '.' && *point == NULL) {
    *point = symbol;
  } else if (*symbol == '.' && *point != NULL) {
    output = SYNTAX_ERROR;
    *start = NULL;
    *end = NULL;
    *point = NULL;
  }
  return output;
}

/// @brief | Проверка указателей на вещественное число |
/// @param  symbol - указатель на символ char *
/// @param  start - указатель на начало
/// @param  point - указатель на точку
/// @param  end - указатель на конец
/// @return
/// 0 - OK;
/// 1 - синтаксическая ошибка
int check_pointers(char *symbol, char **start, char **point, char **end) {
  int output = OK;
  if ((!(isdigit(*symbol)) && *symbol != '.') ||
      ((isdigit(*symbol) || *symbol == '.') && *(symbol + 1) == '\0')) {
    if (*point != NULL && *start != NULL && *end != NULL) {
      if (*point - *start > 0 && *end - *point > 0) {
        *start = NULL;
        *end = NULL;
        *point = NULL;
      } else {
        output = SYNTAX_ERROR;
      }
    } else if (*point != NULL && (*start == NULL || *end == NULL)) {
      output = SYNTAX_ERROR;
    } else if (*point == NULL) {
      *start = NULL;
      *end = NULL;
    }
  }
  return output;
}
