#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <ctype.h>
#include <getopt.h>

int bFlag = 0, nFlag = 0, vFlag = 0, tFlag = 0, eFlag = 0;
int sFlag = 0, StrFlag = 0;

void s21_cat(FILE *file);
int FlagFind(char fl);

#endif  // SRC_CAT_S21_CAT_H_
