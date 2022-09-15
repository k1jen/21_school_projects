#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int func_out = 0;
    if (rows <= 0 && columns <= 0) {
        func_out = 1;
    } else {
        result->rows = rows;
        result->columns = columns;
        result->matrix = calloc(rows, sizeof(double *));
            for (int i = 0; i < rows; i++) {
                result->matrix[i] = calloc(columns, sizeof(double));
            }
    }
    return func_out;
}

void s21_remove_matrix(matrix_t *A) {
    for (int i = 0; i < A->rows; i++)
        if (A->columns > 0 && A->rows > 0 && A->matrix[i] != NULL) free(A->matrix[i]);
    if (A->columns > 0 && A->rows > 0 && A->matrix != NULL) free(A->matrix);
}

int s21_existence_checking(matrix_t *A) {
    int func_out = 0;
    if (A->matrix != NULL) {
        if (A->columns > 0 && A->rows > 0)
            func_out = 1;
    }
    return func_out;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
    int func_out = SUCCESS;
    if (s21_existence_checking(A) == 1 && s21_existence_checking(B) == 1) {
        if ((A->rows == B->rows) && (A->columns == B->columns)) {
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                        double check;
                        check = A->matrix[i][j] - B->matrix[i][j];
                        if (check < 0)
                            check = (-1) * check;
                        if (check > S21_EPS)
                            func_out = FAILURE;
                }
            }
        } else {
            func_out = FAILURE;
        }
    } else {
        func_out = FAILURE;
    }
    return func_out;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int func_out = 0;
    if ((s21_existence_checking(A) == 1) && (s21_existence_checking(B) == 1)) {
        if ((A->rows == B->rows) && (A->columns == B->columns)) {
            s21_create_matrix(A->rows, A->columns, result);
            for (int i = 0; i < result->rows; i++) {
                for (int j = 0; j < result->columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
                }
            }
        } else {
            func_out = 2;
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int func_out = 0;
    if ((s21_existence_checking(A) == 1) && (s21_existence_checking(B) == 1)) {
        if ((A->rows == B->rows) && (A->columns == B->columns)) {
            s21_create_matrix(A->rows, A->columns, result);
            for (int i = 0; i < result->rows; i++) {
                for (int j = 0; j < result->columns; j++) {
                    result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
                }
            }
        } else {
            func_out = 2;
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
    int func_out = 0;
    if (s21_existence_checking(A) == 1) {
        s21_create_matrix(A->rows, A->columns, result);
        for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->columns; j++) {
                result->matrix[i][j] = A->matrix[i][j] * number;
            }
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
    int func_out = 0;
    if ((s21_existence_checking(A) == 1) && (s21_existence_checking(B) == 1)) {
        if (A->columns == B->rows) {
            s21_create_matrix(A->rows, B->columns, result);
            for (int i = 0; i < result->rows; i++) {
                for (int j = 0; j < result->columns; j++) {
                    for (int k = 0; k < A->columns; k++) {
                        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
                    }
                }
            }
        } else {
            func_out = 2;
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
    int func_out = 0;
    if (s21_existence_checking(A) == 1) {
        s21_create_matrix(A->columns, A->rows, result);
        for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->columns; j++) {
                result->matrix[i][j] = A->matrix[j][i];
            }
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
    int func_out = 0;
    if (s21_existence_checking(A) == 1) {
        if (A->rows == A->columns) {
            s21_create_matrix(A->rows, A->columns, result);
            double det = 0.0;
            matrix_t minor = {NULL, 0, 0};
            for (int i = 0; i < A->rows; i++) {
                for (int j = 0; j < A->columns; j++) {
                    s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
                    s21_minor(A, i, j, &minor);
                    s21_determinant(&minor, &det);
                    result->matrix[i][j] = pow(-1, i + j) * det;
                    det = 0.0;
                    s21_remove_matrix(&minor);
                }
            }
        } else {
            func_out = 2;
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

void s21_minor(matrix_t *A, int index_row, int index_column, matrix_t *result) {
    int res_i = 0;
    for (int i = 0; i < result->rows; i++) {
        if (i == index_row) {
            res_i = 1;
        }
        int res_j = 0;
        for (int j = 0; j < result->columns; j++) {
            if (j == index_column) {
                res_j = 1;
            }
            result->matrix[i][j] = A->matrix[i + res_i][j + res_j];
        }
    }
}

int s21_determinant(matrix_t *A, double *result) {
    int func_out = 0;
    if (s21_existence_checking(A) == 1) {
        if (A->rows == A->columns) {
            if (A->rows == 1) {
                *result = A->matrix[0][0];
            } else if (A->rows == 2) {
                *result = A->matrix[0][0] * A->matrix[1][1] -
                    A->matrix[1][0] * A->matrix[0][1];
            } else {
                double det = 0.0;
                matrix_t minor = {NULL, 0, 0};
                for (int j = 0; j < A->columns; j++) {
                    s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
                    s21_minor(A, 0, j, &minor);
                    s21_determinant(&minor, &det);
                    *result += det * A->matrix[0][j] * pow(-1, 2 + j);
                    det = 0;
                    s21_remove_matrix(&minor);
                }
            }
        } else {
            func_out = 2;
        }
    } else {
        func_out = 1;
    }
    return func_out;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
    int func_out = 0;
    if (s21_existence_checking(A) == 1) {
        if (A->rows == A->columns) {
            double det = 0.0;
            s21_determinant(A, &det);
            if (det != 0) {
                matrix_t find_min = {NULL, 0, 0};
                s21_calc_complements(A, &find_min);
                matrix_t find_transp = {NULL, 0, 0};
                s21_transpose(&find_min, &find_transp);
                s21_mult_number(&find_transp, 1 / det, result);
                s21_remove_matrix(&find_min);
                s21_remove_matrix(&find_transp);
            } else {
                func_out = 2;
            }
        } else {
            func_out = 2;
        }
    } else {
        func_out = 1;
    }
    return func_out;
}
