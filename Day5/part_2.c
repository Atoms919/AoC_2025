#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

void remove_overlap(unsigned long long **range_values, int *range_count);

unsigned long long day5(unsigned long long **range_values, int range_count, int *final_count) {
    unsigned long long sum = 0;
    *final_count = range_count;
    remove_overlap(range_values, final_count);
    for (int i = 0; i < *final_count; i++) {
        unsigned long long a = range_values[i][0];
        unsigned long long b = range_values[i][1];
        sum += (b - a + 1);
    }
    return sum;    
}

void remove_overlap(unsigned long long **range_values, int *range_count) {
    bool merged = true;
    while (merged) {
        merged = false;
        for (int i = 0; i < *range_count; i++) {
            for (int j = i + 1; j < *range_count; j++) {
                unsigned long long a1 = range_values[i][0];
                unsigned long long b1 = range_values[i][1];
                unsigned long long a2 = range_values[j][0];
                unsigned long long b2 = range_values[j][1];
                if (((b2 >= a1) && (b2 <= b1)) || ((a2 >= a1) && (a2 <= b1)) ||
                    ((a1 >= a2) && (b1 <= b2)) || ((b1 >= b2) && (a1 <= a2))) {
                    range_values[i][0] = (a1 < a2) ? a1 : a2;
                    range_values[i][1] = (b1 > b2) ? b1 : b2;
                    merged = true;
                    
                    // Free the memory of the range being removed
                    free(range_values[j]);
                    
                    for (int k = j; k < *range_count - 1; k++) {
                        range_values[k] = range_values[k + 1];
                    }
                    (*range_count)--;
                    j--; 
                }
            }
        }
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
                range_values = realloc(range_values, sizeof(unsigned long long *) * (range_index + 1));
            }

            range_values[range_index] = malloc(sizeof(unsigned long long) * 2 );
            range_values[range_index][0] = a;
            range_values[range_index][1] = b;
            range_index++;
            //printf("Read range: %llu - %llu\n", a, b);
            
        }
    }
    int final_count;
    sum = day5(range_values, range_index, &final_count);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);

    fclose(file);

    printf("Sum: %llu\n", sum);

    for (int i = 0; i < final_count; i++) {
        free(range_values[i]);
    }
    free(range_values);

    return 0;
}
