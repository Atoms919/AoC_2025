#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

long day4(char **lines, int line_count) {
    //printf("New iteration\n");
    long sum = 0;
    int line_width = strlen(lines[0]);
    bool removed = false;

    for (int i = 0; i < line_count; i++) {
        char *line = lines[i];
        for (int j = 0; j < strlen(line); j++) {
            if (line[j] != '.') {
                int adjacent_count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue; 
                        
                        int ni = i + di;
                        int nj = j + dj;
                        
                        if (ni >= 0 && ni < line_count && nj >= 0 && nj < strlen(lines[ni])) {
                            if (lines[ni][nj] != '.') {
                                adjacent_count++;
                            }
                        }
                    }
                }
                
                //printf("Tile at (%d, %d) = '%c': %d adjacent non-'.' tiles\n", i, j, line[j], adjacent_count);
                if (adjacent_count < 4) {
                    removed = true;
                    line[j] = '.';
                    sum ++;
                }
            }
        }
    }
    if (removed) sum += day4(lines, line_count);
    return sum;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    clock_t start = clock();

    char line[1024];
    char **lines = NULL;
    int line_count = 0;

    while (fgets(line, sizeof line, file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        lines = realloc(lines, (line_count + 1) * sizeof(char *));
        lines[line_count] = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(lines[line_count], line);
        line_count++;
    }

    printf("%ld\n", day4(lines, line_count));

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }

    free(lines);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_spent);

    fclose(file);
    return 0;
}