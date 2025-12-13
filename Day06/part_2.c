#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

unsigned long long day6(int **values, int value_count, int line_count, char *ops, int ops_count) {
    unsigned long long sum = 0;
    int ops_index = 0;
    unsigned long long sum_line = 0;

    for (int i = 0; i < line_count; i++) {
        int val = 0;
        int power = 1;
        int op = ops[ops_index];
        for (int j = value_count - 1; j >= 0; j--) {
            if (values[j][i] != -1) {
                val += values[j][i] * power;
                power *= 10;
            }
        }
        if (val == 0) {
            //printf("%llu\n", sum_line);
            sum += sum_line;
            sum_line = 0;
            ops_index++;
        }
        else {
            if (op == '+') {
                sum_line += val;
            }
            else if (op == '*') {
                if (sum_line == 0) {
                    sum_line = 1;
                }
                sum_line *= val;
            }
            //printf("%d %c ", val, op);
        }
    }

    return sum;
    
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[8192];
    unsigned long long sum = 0;
    bool read_values = true;
    int **values = malloc(sizeof(int *) * 1);
    char *ops = malloc(sizeof(char) * 1);
    int values_index = 0;
    int ops_index = 0;
    int col_index = 0;
    int num_cols = 0;

    while (fgets(line, sizeof line, file)) {
        char *ptr = line;
        col_index = 0;
        
        if (read_values) {
            if (values_index >= 1) {
                values = realloc(values, sizeof(int *) * (values_index + 1));
            }
            values[values_index] = malloc(sizeof(int) * 1);
        }
        while (*ptr) {
            char *end;
            int val = strtol(ptr, &end, 10);
            if (*ptr == '\n') {
                break;
            }
            else if (ptr == end){
                read_values = false;
            }
            if (read_values) {
                while (ptr != end) {
                    if (col_index >= 1) {
                        values[values_index] = realloc(values[values_index], sizeof(int) * (col_index + 1));
                    }
                    if (*ptr == ' ') {
                        values[values_index][col_index] = -1;
                    }
                    else {
                        values[values_index][col_index] = *ptr - '0';
                    }
                    col_index++;
                    ptr++;
                }
                while (*ptr == ' ') {
                    if (col_index >= 1) {
                        values[values_index] = realloc(values[values_index], sizeof(int) * (col_index + 1));
                    }
                    values[values_index][col_index] = -1;
                    col_index++;
                    ptr++;
                }
                if (*ptr == '\n' || *ptr == '\0') {
                    if (col_index >= 1) {
                        values[values_index] = realloc(values[values_index], sizeof(int) * (col_index + 1));
                    }
                    values[values_index][col_index] = -1;
                    col_index++;
                }
            }
            else {
                if (*ptr == ' ') {
                    ptr++;
                }
                else if (*ptr != '\n' && *ptr != '\0') {
                    if (ops_index >= 1) {
                        ops = realloc(ops, sizeof(char) * (ops_index + 1));
                    }
                    ops[ops_index] = *ptr;
                    ops_index++;
                    ptr++;
                }
                else {
                    break;
                }
            }
        }
        if (read_values) {
            if (col_index > num_cols) num_cols = col_index;
            values_index++;
        }
    }
    sum = day6(values, values_index, num_cols, ops, ops_index);
    fclose(file);

    printf("Sum: %llu\n", sum);

    for (int i = 0; i < values_index; i++) {
        free(values[i]);
    }
    free(values);

    free(ops);

    return 0;
}
