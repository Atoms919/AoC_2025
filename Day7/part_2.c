#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void afficher(long long **lines, int line_count, int line_length) {
    for (int i = 0; i < line_count; i++) {
        for(int j = 0; j < line_length; j++) {
            if (lines[i][j] != -1) printf("%lld ", lines[i][j]);
            else printf("^ " );
        }
        printf("\n");
    }
}

long long day7(long long **lines, int line_count, int line_length) {
    
    
    for (int i = 0; i < line_count; i++) {
        for (int j = 0; j < line_length; j++) {
            if (lines[i][j] > 0) {
                if (i + 1 < line_count && lines[i+1][j] != -1) {
                    lines[i+1][j] += lines[i][j];
                }
                if (i + 1 < line_count && lines[i+1][j] == -1) {
                    lines[i+1][j+1] += lines[i][j];
                    lines[i+1][j-1] += lines[i][j];
                }
            }
        }
    }
    //afficher(lines, line_count, line_length);
    long long count = 0;
    for (int j = 0; j < line_length; j++) {
        count += lines[line_count - 1][j];
    }
    return count;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    long long **lines = NULL;
    int line_count = 0;
    int line_length = 0;
    
    while (fgets(line, sizeof line, file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (line_length == 0) {
            line_length = strlen(line);
        }

        lines = realloc(lines, (line_count + 1) * sizeof(long long *));
        lines[line_count] = malloc((line_length + 1) * sizeof(long long));
        for (int j = 0; j <= line_length; j++) {
            if (line[j] == 'S') {
                lines[line_count][j] = 1;
            } 
            else if (line[j] == '^') {
                lines[line_count][j] = -1;
            }
            else {
                lines[line_count][j] = 0;
            }
        }
        line_count++;
    }

    //afficher(lines, line_count, line_length);
    long long result = day7(lines, line_count, line_length);
    //afficher(lines, line_count, line_length);
    printf("%lld\n", result);

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }

    free(lines);

    fclose(file);
    return 0;
}