#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long long **memo = NULL;
int memo_rows = 0;
int memo_cols = 0;

void afficher(char **lines, int line_count) {
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", lines[i]);
    }
}
long long day7(char **lines, int line_count, int line_pos, int col_pos) {
    if (line_pos < memo_rows && col_pos < memo_cols && memo[line_pos][col_pos] != -1) {
        return memo[line_pos][col_pos];
    }
    
    int final_pos = line_pos;
    while (final_pos + 1 < line_count && lines[final_pos + 1][col_pos] != '^') {
        final_pos++;
    }
    
    long long count = 0;
    
    if (final_pos + 1 >= line_count) {
        count = 1;
    }
    else {
        char *line = lines[final_pos];
        if (col_pos - 1 >= 0) {
            count += day7(lines, line_count, final_pos, col_pos - 1);
        }
        if (col_pos + 1 < strlen(line)) {
            count += day7(lines, line_count, final_pos, col_pos + 1);
        }
    }
    
    if (line_pos < memo_rows && col_pos < memo_cols) {
        memo[line_pos][col_pos] = count;
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
    
    memo_rows = line_count;
    memo_cols = (line_count > 0) ? strlen(lines[0]) : 0;
    
    memo = malloc(memo_rows * sizeof(long long *));
    for (int i = 0; i < memo_rows; i++) {
        memo[i] = malloc(memo_cols * sizeof(long long));
        for (int j = 0; j < memo_cols; j++) {
            memo[i][j] = -1;
        }
    }
    
    long long result = 0;
    char *start = lines[0];
    for (int j = 0; j < strlen(start); j++) {
        if (start[j] == 'S') {
            result = day7(lines, line_count, 0, j);
            break;
        }
    }
    printf("%lld\n", result);

    // Free memoization table
    for (int i = 0; i < memo_rows; i++) {
        free(memo[i]);
    }
    free(memo);

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }

    free(lines);

    fclose(file);
    return 0;
}