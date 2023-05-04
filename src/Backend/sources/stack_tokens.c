#include "../headers/stack_tokens.h"

/// @brief | Заполняет поля лексемы |
/// @param  token_object - указатель на лексему token *
/// @param  status - статус лексемы int
/// @param  value - значение вещественного числа double
/// @param  name - имя оператора(функции) char *
void init_token(token *token_object, int status, double value, char *name) {
  if (token_object != NULL) {
    token_object->status = status;
    token_object->value = value;
    strcpy(token_object->name, name);
    token_object->priority = get_priority(name[0]);
  }
}

/// @brief | Возвращает приоритет операции |
/// @param  symbol - первый символ оператора(функции)
/// @return приоритет операции int
int get_priority(char symbol) {
  int priority = 0;
  switch (symbol) {
    case '+':
    case '-':
      priority = e_sum_priority;
      break;
    case '*':
    case '/':
      priority = e_mul_priority;
      break;
    case '^':
      priority = e_pow_priority;
      break;
    case 'm':
      priority = e_mod_priority;
      break;
    case ')':
      priority = e_right_bracket_priority;
      break;
    default:
      priority = e_left_bracket_priority;
  }
  return priority;
}

/// @brief | Инициализация стека лексем |
/// @param  token_object- добавляемая лексема
/// @return указатель на верхний элемент
stack_tokens *init_stack() {
  stack_tokens *head = (stack_tokens *)malloc(sizeof(stack_tokens));
  if (head != NULL) {
    head->prev_token = NULL;
    head->size = 0;
  }
  return head;
}

/// @brief | Создание указателя на новый элемент стека |
/// @param  token_object- добавляемая лексема token
/// @return указатель на новый элемент
stack_tokens *create_new_element_stack(token token_object) {
  stack_tokens *head = (stack_tokens *)malloc(sizeof(stack_tokens));
  if (head != NULL) {
    head->token_object = token_object;
    head->prev_token = NULL;
    head->size = 0;
  }
  return head;
}

/// @brief | Добавление новой лексемы в стек |
/// @param  head - указатель на верхний элемент
/// @param  token_object - добавляемая лексема
/// @return указатель на верхний элемент
stack_tokens *push_token(stack_tokens *head, token token_object) {
  stack_tokens *output = head;
  if (head != NULL && head->size == 0) {
    head->token_object = token_object;
    ++(head->size);
  } else if (head != NULL && head->size != 0) {
    stack_tokens *new_token = create_new_element_stack(token_object);
    if (new_token != NULL) {
      output = new_token;
      new_token->prev_token = head;
      new_token->size = head->size + 1;
    }
  }
  return output;
}

/// @brief | Выталкивание верхней лексемы из стека |
/// @param  head - указатель на верхний элемент
/// @return верхняя лексема
token pop_token(stack_tokens **head) {
  token output = (*head)->token_object;
  if ((*head)->size == 1) {
    (*head)->size = 0;
  } else {
    stack_tokens *tmp = (*head);
    (*head) = (*head)->prev_token;
    free(tmp);
  }
  return output;
}

/// @brief | Очистка стека |
/// @param  head - указатель на верхний элемент
void free_stack(stack_tokens *head) {
  if (head != NULL) {
    while (head->prev_token != NULL) {
      pop_token(&head);
    }
    free(head);
    head = NULL;
  }
}
