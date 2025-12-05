#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

void order(unsigned long long **range_values, int *range_count);

unsigned long long day5(unsigned long long **range_values, int range_count) {
    unsigned long long sum = 0;
    order(range_values, &range_count);

    int state = 0;
    unsigned long long start = 0;
    
    for (int i = 0; i < range_count; i++) {
        unsigned long long a = range_values[i][0];
        unsigned long long b = range_values[i][1];
        //printf("Point: %llu, Type: %llu\n", a, b);
        if (b == 0) { 
            if (state == 0) {
                start = a;
            }
            state++;
        } else { 
            state--;
            if (state == 0) {
                //printf("Range: %llu - %llu\n", start, a);
                sum += (a - start + 1);
            }
        }
    }
    return sum;    
}

void order(unsigned long long **range_values, int *range_count) {
    bool swapped = false;
    for (int i = 0; i < *range_count; i++) {
        for (int j = 0; j < *range_count - i - 1; j++) {
            if (range_values[j][0] > range_values[j + 1][0]) {
                unsigned long long *temp = range_values[j];
                range_values[j] = range_values[j + 1];
                range_values[j + 1] = temp;
                swapped = true;
            }
            else if (range_values[j][0] == range_values[j + 1][0]) {
                if (range_values[j][1] > range_values[j + 1][1]) {
                    unsigned long long *temp = range_values[j];
                    range_values[j] = range_values[j + 1];
                    range_values[j + 1] = temp;
                    swapped = true;
                }
            }
        }
        if (!swapped) {
            break;
        }
        swapped = false;
    }
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }
    clock_t start = clock();
    
    char line[1024];
    unsigned long long sum = 0;
    unsigned long long **range_values = malloc(sizeof(unsigned long long *) * 2);
    int range_index = 0;

    while (fgets(line, sizeof line, file)) {
        char *ptr = line;
        if (*ptr == '\n') break;
        while (*ptr) {
            char *end;
            unsigned long long a = strtoull(ptr, &end, 10);
            ptr = end + 1;
            unsigned long long b = strtoull(ptr, &end, 10);
            ptr = end + 1;

            if (range_index >= 2) {
                range_values = realloc(range_values, sizeof(unsigned long long *) * (range_index + 2));
            }

            range_values[range_index] = malloc(sizeof(unsigned long long) * 2 );
            range_values[range_index][0] = a;
            range_values[range_index][1] = 0;
            range_index++;
            range_values[range_index] = malloc(sizeof(unsigned long long) * 2 );
            range_values[range_index][0] = b;
            range_values[range_index][1] = 1;
            range_index++;
        }
    }
    sum = day5(range_values, range_index);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);

    fclose(file);

    printf("Sum: %llu\n", sum);

    for (int i = 0; i < range_index; i++) {
        free(range_values[i]);
    }
    free(range_values);

    return 0;
}
