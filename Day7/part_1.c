#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void split_beam(char **lines, int line_count, int line_pos, int col_pos) {
    if (lines[line_pos][col_pos] == '|') {
        return;
    }
    while (line_pos + 1 < line_count && lines[line_pos][col_pos] != '^') {
        lines[line_pos][col_pos] = '|';
        line_pos++;
    }
    if (line_pos + 1 < line_count) {
        char *line = lines[line_pos];
        if (col_pos - 1 >= 0) {
            lines[line_pos][col_pos-1] = '|';
            split_beam(lines, line_count, line_pos + 1, col_pos - 1);
        }
        if (col_pos + 1 < strlen(line)) {
            lines[line_pos][col_pos+1] = '|';
            split_beam(lines, line_count, line_pos + 1, col_pos + 1);
        }
    }
}

int day7(char **lines, int line_count) {
    int count = 0;

    char *line = lines[0];
    for (int j = 0; j < strlen(line); j++) {
        if (line[j] == 'S') {
            line[j] = '|';
            split_beam(lines, line_count, 1, j);
            break;
        }
    }
    
    for (int i = 1; i < line_count; i++) {
        char *prev_line = lines[i-1];
        char *line = lines[i];
        for (int j = 0; j < strlen(line); j++) {
            if (line[j] == '^' && prev_line[j] == '|') {
                count++;
            }
        }
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

    int result = day7(lines, line_count);
    printf("%d\n", result);

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }

    free(lines);

    fclose(file);
    return 0;
}