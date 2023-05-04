#ifndef SMARTCALC_V1_0_SRC_BACKEND_HEADERS_CALCULATION_H_
#define SMARTCALC_V1_0_SRC_BACKEND_HEADERS_CALCULATION_H_

#include <math.h>

#include "common.h"
#include "stack_tokens.h"

#define NEW_M_PI 3.14159265358979323846
#define NEW_M_E 2.7182818284590452354
#define AMOUNT_MATH_FUNCTIONS 9
#define NAMES_MATH_FUNCTIONS cos, tan, sin, sqrt, log, log10, acos, asin, atan
#define NAMES_TOKEN_FUNCTIONS \
  { "c", "t", "si", "sq", "ln", "lg", "ac", "as", "at" }
#define AMOUNT_OPERATORS_FUNCTIONS 6
#define NAMES_OPERATORS_FUNCTIONS my_sum, my_sub, my_mul, my_div, pow, fmod
#define NAMES_TOKEN_OPERATORS \
  { "+", "-", "*", "/", "^", "m" }

int calculation(token *reverse_polish_notation_array, const int amount_tokens,
                const double x_value, double *result);
void set_special_number(token *reverse_polish_notation_array,
                        const int amount_tokens, const double x_value);
stack_tokens *functions_workspace(const token token_object,
                                  stack_tokens *stack_value);
double get_value_special_number(char symbol, const double x_value);
double functions_selector(const char *name_function, const double value);
stack_tokens *operators_workspace(const token token_object,
                                  stack_tokens *stack_value);
double operators_selector(const char *name_function, const double value_1,
                          const double value_2);
double my_sum(double value_1, double value_2);
double my_sub(double value_1, double value_2);
double my_mul(double value_1, double value_2);
double my_div(double value_1, double value_2);

#endif  // SMARTCALC_V1_0_SRC_BACKEND_HEADERS_CALCULATION_H_
