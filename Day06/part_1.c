#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

unsigned long long day6(int **values, int value_count, int line_count, char *ops, int ops_count) {
    unsigned long long sum = 0;
    unsigned long long sum_line = 0;
    printf("Line count: %d\n", line_count);
    printf("Value count: %d\n", value_count);
    printf("Ops count: %d\n", ops_count);
    for (int i = 0; i < line_count; i++) {
        if (ops[i] == '+') {
            sum_line = 0;
        }
        else if (ops[i] == '*') {
            sum_line = 1;
        }
        
        for (int j = 0; j < value_count; j++) {
            //printf("%d\n", values[j][i]);
            if (ops[i] == '+') {
                sum_line += values[j][i];
            }
            else if (ops[i] == '*') {
                sum_line *= values[j][i];
            }
        }
        //printf("\n");
        sum += sum_line;
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
                //printf("Read value: %d\n", val);
                ptr = end;
                while (*ptr == ' ') ptr++;

                if (col_index >= 1) {
                    values[values_index] = realloc(values[values_index], sizeof(int) * (col_index + 1));
                }
                values[values_index][col_index] = val;
                col_index++;
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
