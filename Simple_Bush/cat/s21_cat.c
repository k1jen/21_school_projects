#include "s21_cat.h"

int main(int argc, char* argv[]) {
    int flagCheck = 0;
    static struct option long_options[] = {
        {"number-nonblank", 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {0, 0, 0, 0}
    };
    opterr = 0;
    while (!flagCheck) {
        int fl;
        fl = getopt_long(argc, argv, "beEnstTv", long_options, NULL);
        if (fl == -1)
            break;
        else
            flagCheck = FlagFind(fl);
    }
    if (!flagCheck) {
        argv += optind;
        while (*argv != NULL) {
            FILE *fileOpen;
            if ((fileOpen = fopen(*argv, "r")) != NULL) {
                s21_cat(fileOpen);
                fclose(fileOpen);
            } else {
                printf("s21_cat: %s No such file or directory ", *argv);
            }
            argv++;
        }
    } else {
        printf("s21_cat: No such flag ");
    }
    return 0;
}

void s21_cat(FILE *file) {
    int sym, predSym;
    long unsigned lineNumb = 1;
    for (predSym = '\n'; (sym = getc(file)) != EOF; predSym = sym) {
        if (predSym == '\n') {
            if (sFlag) {
                if (sym == '\n') {
                    if (StrFlag)
                        continue;
                    StrFlag = 1;
                } else {
                    StrFlag = 0;
                }
            }
            if (nFlag) {
                if (!bFlag || sym != '\n') {
                    printf("%6lu\t", lineNumb++);
                }
            }
        }
        if (sym == '\n') {
            if (eFlag)
                printf("%c", '$');
        } else if (sym == '\t') {
            if (tFlag) {
                printf("^I");
                continue;
            }
        } else if (vFlag) {
            if (!isascii(sym)) {
                printf("M-");
                sym = toascii(sym);
            }
            if (iscntrl(sym)) {
                printf("^%c", sym == '\177' ? '?' : sym | 0100);
                continue;
            }
        }
        printf("%c", sym);
    }
}

int FlagFind(char fl) {
    int flag = 0;
    switch (fl) {
        case 'b':
            bFlag = nFlag = 1;
            break;
        case 'e':
            eFlag = vFlag = 1;
            break;
        case 'E':
            eFlag = 1;
            break;
        case 'n':
            nFlag = 1;
            break;
        case 's':
            sFlag = 1;
            break;
        case 't':
            tFlag = vFlag = 1;
            break;
        case 'T':
            tFlag = 1;
            break;
        case 'v':
            vFlag = 1;
            break;
        case '?':
        default:
            flag = 1;
    }
    return flag;
}
