#ifndef SMARTCALC_V1_0_SRC_BACKEND_HEADERS_VALIDATION_H_
#define SMARTCALC_V1_0_SRC_BACKEND_HEADERS_VALIDATION_H_

#include "common.h"

#define AMOUNT_CHECK_FUNCTIONS 4
#define NAMES_CHECK_FUNCTIONS                                            \
  check_allowed_characters, check_amount_brackets, check_name_operators, \
      check_points
#define AMOUNT_OPERATORS 13
#define NAMES_OPERATORS                                                       \
  {                                                                           \
    "mod", "cos(", "sin(", "tan(", "acos(", "asin(", "atan(", "sqrt(", "ln(", \
        "log(", "x", "pi", "e"                                                \
  }

int input_array_validation(char *input_array);
int check_allowed_characters(char *input_array);
int check_amount_brackets(char *input_array);
int check_name_operators(char *input_array);
int compare_name_operators(char *potential_operator);
int check_points(char *input_array);
int find_point(char *symbol, char **start, char **point, char **end);
int check_pointers(char *symbol, char **start, char **point, char **end);

#endif  // SMARTCALC_V1_0_SRC_BACKEND_HEADERS_VALIDATION_H_
