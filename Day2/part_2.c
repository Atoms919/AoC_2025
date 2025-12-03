#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int not_in_array(int value, int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return 0;
        }
    }
    return 1;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    unsigned long long sum = 0;
    int capacity = 1000;
    int size = 0;
    int *arr = malloc(capacity * sizeof *arr);

    while (fgets(line, sizeof line, file)) {
        char *ptr = line;
        while (*ptr) {
            char *end;
            unsigned long long a = strtoull(ptr, &end, 10);
            if (ptr == end) break;
            ptr = end + 1;
            unsigned long long b = strtoull(ptr, &end, 10);

            //printf("Range: %llu - %llu\n", a, b);

            for (unsigned long long j = 0; j <= b-a; j++) {
                char a_j_str[32], num_str[32], invalid_str_1[64], invalid_str[64];
                int i, len;
                unsigned long long invalid = 0;

                sprintf(a_j_str, "%llu", a + j);
                len = strlen(a_j_str);

                for (int i = 1; i<=len/2; i++) {
                    if (len%i == 0) 
                    {
                        sprintf(num_str, "%llu", a + j);
                        strncpy(invalid_str_1, num_str, i);
                        invalid_str_1[i] = '\0';
                        strcpy(invalid_str, invalid_str_1);
                        while (strlen(invalid_str) < len) {
                            strcat(invalid_str, invalid_str_1);
                            invalid = strtoull(invalid_str, NULL, 10);
                        }
                        

                        //printf("Checking invalid: %llu\n", invalid);

                        if (invalid <= b && invalid >= a) {
                            if (not_in_array(invalid, arr, size)) {
                                if (size == capacity) {
                                    capacity += 1000;
                                    arr = realloc(arr, capacity * sizeof *arr);
                                }
                                arr[size] = invalid;
                                size++;
                                
                                sum += invalid;
                                //printf("Invalid: %llu\n", invalid);
                            }                            
                        }
                    }                
                }
                if (*end == ',') end++;
                ptr = end;
            }
        }
    }
    fclose(file);

    printf("Sum: %llu\n", sum);
    return 0;
}
