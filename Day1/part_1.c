#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int main() {
    FILE *file = fopen("input.txt", "r");
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

        if (first_char == 'R') {
            pos+=atoi(rest);
        } else if (first_char == 'L') {
            pos-=atoi(rest);  
        }

        pos = pos % 100;
        if (pos < 0) {
            pos += 100;
        }

        if (pos == 0) {
            sum ++;
        }
    }

    free(buffer);
    fclose(file);

    printf("Number of 0s: %d\n", sum);
    return 0;
}

