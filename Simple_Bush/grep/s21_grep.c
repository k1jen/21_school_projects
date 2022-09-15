#include "s21_grep.h"

int main(int argc, char **argv) {
    list *pattern = NULL;
    list *file_find = NULL;
    parser(argc, argv, &pattern, &file_find);
    find_and_out(pattern, file_find);
    clear(pattern);
    clear(file_find);
    pattern = NULL;
    file_find = NULL;
    return 0;
}

void parser(int argc, char **argv, list **pattern, list **file_find) {
    int flag_find = 0, pattern_count = 0, file_count = 0;
    while ((flag_find = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != EOF) {
        switch (flag_find) {
            case 'i':
                flag_i = REG_ICASE;
                break;
            case 'v':
                flag_v = 1;
                break;
            case 'c':
                flag_c = 1;
                break;
            case 'l':
                flag_l = 1;
                break;
            case 'n':
                flag_n = 1;
                break;
            case 'h':
                flag_h = 1;
                break;
            case 's':
                flag_s = 1;
                break;
            case 'o':
                flag_o = 1;
                break;
            case 'e':
                *pattern = toDo(optarg, *pattern);
                pattern_count++;
                break;
            case 'f':
                if (f_fl_func(pattern) == 1)
                    pattern_count++;
                break;
            case '?':
                if (!flag_s)
                    printf("usage: %s [-chilnosv] [-e pattern] [-f file]", argv[0]);
                break;
        }
    }
    for (; optind < argc; optind++) {
        if (!flag_h && file_count > 0) {
            flag_h = -1;
        }
        if (!pattern_count) {
            *pattern = toDo(argv[optind], *pattern);
            pattern_count++;
        } else {
            *file_find = toDo(argv[optind], *file_find);
            file_count++;
        }
    }
}

int f_fl_func(list **pattern) {
    int YesNo_flag = 0;
    FILE *pattern_file = NULL;
    if ((pattern_file = fopen(optarg, "r")) != NULL) {
        char *line = NULL;
        size_t buff_size = 0;
        ssize_t line_size = 0;
        while ((line_size = getline(&line, &buff_size, pattern_file)) >= 0) {
            if (line[line_size - 1] == '\n')
                line[line_size - 1] = '\0';
            *pattern = toDo(line, *pattern);
        }
        free(line);
        fclose(pattern_file);
        pattern_file = NULL;
        YesNo_flag = 1;
    } else {
        if (!flag_s)
            printf("grep: %s: No such file or directory\n", optarg);
    }
    return YesNo_flag;
}

void find_and_out(list *pattern, list *file_find) {
    char *line = NULL;
    if (flag_v && flag_o)
        flag_o = 0;
    while (file_find != NULL) {
        FILE *file;
        if ((file = fopen(file_find->str, "r")) != NULL) {
            ssize_t line_size;
            size_t buff_size;
            int line_number = 0, line_count = 0, match_flag = 0;
            while ((line_size = getline(&line, &buff_size, file)) >= 0) {
                line_number++;
                list *pattern_for_line = pattern;
                regex_t compiled_regular;
                regmatch_t regmatch;
                while (pattern_for_line != NULL) {
                    if ((regcomp(&compiled_regular, pattern_for_line->str, flag_i)) == 0) {
                        int match = regexec(&compiled_regular, line, 1, &regmatch, 0);
                        if (match == 0 && match_flag == 0) {
                            if (flag_o && !flag_v && !flag_c && !flag_l)
                                o_fl_func(file_find, compiled_regular, regmatch, line, line_number);
                            match_flag = 1;
                        }
                        pattern_for_line = pattern_for_line->next;
                    }
                    regfree(&compiled_regular);
                }
                if (flag_v) {
                    if (match_flag == 1)
                        match_flag = 0;
                    else
                        match_flag = 1;
                }
                if (match_flag == 1) {
                    if (line[line_size - 1] == '\n')
                        line[line_size - 1] = '\0';
                    if (!flag_c && !flag_l && !flag_o) {
                        if (flag_h == -1)
                            printf("%s:", file_find->str);
                        if (flag_n)
                            printf("%d:", line_number);
                        printf("%s\n", line);
                    }
                    match_flag = 0;
                    line_count++;
                }
            }
            if (flag_c) {
                if (flag_l && line_count)
                    line_count = 1;
                if (flag_h == -1)
                    printf("%s:", file_find->str);
                printf("%d\n", line_count);
            }
            if (flag_l && line_count) {
                printf("%s\n", file_find->str);
            }
            line_number = 0;
            line_count = 0;
            fclose(file);
            file_find = file_find->next;
        } else {
            if (!flag_s)
                fprintf(stderr, "grep: %s: No such file or directory\n", file_find->str);
            file_find = file_find->next;
        }
    }
    free(line);
    line = NULL;
}

void o_fl_func(list *file_find, regex_t compiled_regular, regmatch_t regmatch, char *line, int line_number) {
    char *found_txt = line;
    if (flag_h == -1)
        printf("%s:", file_find->str);
    if (flag_n)
        printf("%d:", line_number);
    while (regexec(&compiled_regular, found_txt, 1, &regmatch, 0) == 0) {
        for (size_t i = regmatch.rm_so; i < (size_t) regmatch.rm_eo; i++)
            printf("%c", found_txt[i]);
        printf("\n");
        found_txt = found_txt + regmatch.rm_eo;
    }
}

list *toDo(char *str, list *head) {
    if (head == NULL) {
        head = create(str);
    } else {
        add_new(str, head);
    }
    return head;
}

void *create(char *str) {
    list *tmp = (list *) malloc(sizeof(list));
    strcpy(tmp->str, str);
    tmp->next = NULL;
    return tmp;
}

void add_new(char *str, list *head) {
    list *tmp = (list *) malloc(sizeof(list));
    strcpy(tmp->str, str);
    tmp->next = NULL;
    list *pt = head;
    while (pt->next != NULL)
        pt = pt->next;
    pt->next = tmp;
}

void clear(list *head) {
    while (head != NULL) {
        list *temp = head;
        head = head->next;
        free(temp);
    }
}
