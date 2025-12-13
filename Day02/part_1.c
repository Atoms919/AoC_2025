#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    unsigned long long sum = 0;

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
                char a_j_str[32], num_str[32], invalid_str[64];
                int i, len;
                unsigned long long invalid = 0;

                sprintf(a_j_str, "%llu", a + j);
                len = strlen(a_j_str);
                i = len / 2;

                sprintf(num_str, "%llu", a + j);
                strncpy(invalid_str, num_str, i);
                invalid_str[i] = '\0';
                strcat(invalid_str, invalid_str);
                invalid = strtoull(invalid_str, NULL, 10);

                if (invalid <= b && invalid >= a+j) {
                    sum += invalid;
                    j = invalid - a;
                }
                else if (invalid > b) {
                    break;
                }
            }
            if (*end == ',') end++;
            ptr = end;
        }
    }

    fclose(file);

    printf("Sum: %llu\n", sum);
    return 0;
}
