#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#define BUFFSIZE 1024

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>
#include <stdlib.h>

int flag_i = 0, flag_c = 0, flag_l = 0, flag_n = 0;
int flag_h = 0, flag_s = 0, flag_o = 0, flag_v = 0;

typedef struct node {
  char str[1024];
  struct node *next;
} list;

void *create(char *str);
void add_new(char *str, list *head);
void clear(list *head);
list *toDo(char *str, list *head);
void parser(int argc, char **argv, list **pattern, list **file_find);
int f_fl_func(list **pattern);
void find_and_out(list *pattern, list *file_find);
void o_fl_func(list *file_find, regex_t compiled_regular, regmatch_t regmatch, char *line, int line_number);

#endif  // SRC_GREP_S21_GREP_H_
