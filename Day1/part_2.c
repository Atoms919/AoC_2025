#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int main() {
    FILE *file = fopen("input_1.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char *buffer = malloc(1024);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    int sum = 0;
    int pos = 50;
    while (fgets(buffer, 1024, file)) {
        char first_char = buffer[0];      
        char *rest = buffer + 1;
        int pos_start = pos;

        if (first_char == 'R') {
            pos+=atoi(rest);
        } else if (first_char == 'L') {
            pos-=atoi(rest);  
        }

        if (pos < 0) {
            sum += (-pos) / 100;
            if (pos_start != 0) {
                sum += 1;
            }
        }
        
        if (pos == 0) {
            sum ++;
        }
        else if (pos >= 100) {
            sum += (pos) / 100;
        }

        pos = pos % 100;
        if (pos < 0) {
            pos += 100;
        }
    }

    free(buffer);
    fclose(file);

    printf("Passed on 0: %d\n", sum);
    return 0;
}

