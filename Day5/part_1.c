#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

unsigned long long day5(unsigned long long **range_values, int range_count, unsigned long long *values, int value_count) {
    unsigned long long sum = 0;

    for (int i = 0; i < value_count; i++) {
        unsigned long long val = values[i];
        for (int i = 0; i < range_count; i++) {
            unsigned long long a = range_values[i][0];
            unsigned long long b = range_values[i][1];
            if (val >= a && val <= b) {
                sum ++;
                break;
            }
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

    char line[1024];
    unsigned long long sum = 0;
    bool ranges = true;
    unsigned long long **range_values = malloc(sizeof(unsigned long long *) * 2);
    unsigned long long *values = malloc(sizeof(unsigned long long) * 2);
    int range_index = 0;
    int values_index = 0;

    while (fgets(line, sizeof line, file)) {
        char *ptr = line;
        if (*ptr == '\n') ranges = false;
        while (*ptr) {
            if (ranges) {
                char *end;
                unsigned long long a = strtoull(ptr, &end, 10);
                ptr = end + 1;
                unsigned long long b = strtoull(ptr, &end, 10);
                ptr = end + 1;

                if (range_index >= 2) {
                    range_values = realloc(range_values, sizeof(unsigned long long *) * (range_index + 1));
                }

                range_values[range_index] = malloc(sizeof(unsigned long long) * 2 );
                range_values[range_index][0] = a;
                range_values[range_index][1] = b;
                range_index++;
                //printf("Read range: %llu - %llu\n", a, b);
            }
            else {
                char *end;
                unsigned long long val = strtoull(ptr, &end, 10);
                if (ptr == end) break;
                
                if (values_index >= 2) {
                    values = realloc(values, sizeof(unsigned long long) * (values_index + 1));
                }
                
                values[values_index] = val;
                ptr = end + 1;
                values_index++;
                //printf("Read value: %llu\n", val);
            }
        }
    }
    sum = day5(range_values, range_index, values, values_index);

    fclose(file);

    printf("Sum: %llu\n", sum);

    free(values);
    for (int i = 0; i < range_index; i++) {
        free(range_values[i]);
    }
    free(range_values);

    return 0;
}
