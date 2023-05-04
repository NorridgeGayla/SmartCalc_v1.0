#include "../headers/common.h"

/// @brief | Создает указатель на массив символов |
/// @param length - длина массива const int
/// @return
/// указатель на  массив символов
char *init_char_array(const int length) {
  return (char *)calloc(length, sizeof(char));
}

/// @brief | Изменяет размер массива символов |
/// @param  char_array - массив символов char *
/// @param prev_length - указатель на исходный размер массива int *
/// @return
/// измененный массив символов
char *realloc_char_array(char *char_array, int *prev_length) {
  *prev_length *= 2;
  char_array = (char *)realloc(char_array, *prev_length * sizeof(char));
  return char_array;
}
