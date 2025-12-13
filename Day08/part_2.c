#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <time.h>

typedef struct {   
    long x;
    long y;
    long z;
    int index;
} Position; 

typedef struct {
    int index_a;
    int index_b;
    float distance;
} DistancePair;

DistancePair* memo = NULL;
int memo_size = 0;
int memo_capacity = 0;

void init_memo() {
    memo_capacity = 1000;
    memo_size = 0;
    memo = malloc(memo_capacity * sizeof(DistancePair));
}

void free_memo() {
    free(memo);
    memo = NULL;
    memo_size = 0;
    memo_capacity = 0;
}

DistancePair pop() {
    DistancePair top = {-1, -1, FLT_MAX};
    if (memo_size > 0) {
        top = memo[memo_size - 1];
    }
    return top;
}

void push(int index_a, int index_b, float distance) {
    if (memo_size >= memo_capacity) {
        memo_capacity += 1000;
        memo = realloc(memo, memo_capacity * sizeof(DistancePair));
    }
    memo[memo_size].index_a = index_a;
    memo[memo_size].index_b = index_b;
    memo[memo_size].distance = distance;
    memo_size++;
}

void min_distance(float** dist_matrix, int position_count, int* index_a, int* index_b) {
    float min_dist = FLT_MAX;
    int i_min = 0;
    int j_min = 0;

    *index_a = -1;
    *index_b = -1;

    if (memo_size > 0) {
        DistancePair top = pop();
        min_dist = top.distance;
        i_min = top.index_a;
        *index_a = top.index_a;
        j_min = top.index_b;
        *index_b = top.index_b;
        memo_size--;
    }
    
    for (int i = i_min; i < position_count; i++) {
        for (int j = j_min; j < i; j++) {
            if (dist_matrix[i][j] < min_dist) {
                push(i, j, dist_matrix[i][j]);

                min_dist = dist_matrix[i][j];
                *index_a = i;
                *index_b = j;
            }
        }
        j_min = 0;
    }
    if (memo_size > 0) {
        pop();
    }
}

void order_groups_by_size(int** tab, int col, int* row) {
    for (int i = 0; i < col - 1; i++) {
        for (int j = 0; j < col - i - 1; j++) {
            if (row[j] < row[j + 1]) {
                int* temp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = temp;

                int temp_size = row[j];
                row[j] = row[j + 1];
                row[j + 1] = temp_size;
            }
        }
    }
}

int findIndex(int** tab, int col, int* row, int index) {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row[i]; j++) {
            if (tab[i][j] == index) {
                return i;
            }
        }
    }
    return -1;
}

long long day8(Position **positions, int position_count) {
    init_memo();

    int** tab = (int**)malloc(sizeof(int*));
    tab[0] = (int*)malloc(2*sizeof(int));
    int col = 0;
    int *row = (int*)malloc(sizeof(int));
    row[0] = 0;

    float **dist_matrix = (float **)malloc(position_count * sizeof(float*));
    for (int i = 0; i < position_count; i++) {
        dist_matrix[i] = (float *)malloc(position_count * sizeof(float));
        for (int j = 0; j < position_count; j++) {
            if (i == j) {
                dist_matrix[i][j] = FLT_MAX;
            } else {
                float dx = positions[i]->x - positions[j]->x;
                float dy = positions[i]->y - positions[j]->y;
                float dz = positions[i]->z - positions[j]->z;
                dist_matrix[i][j] = sqrt((float)(dx*dx + dy*dy + dz*dz));
            }
        }
    }

    int index0 = 0;
    int index1 = 0;

    int index_col0 = -1;
    int index_col1 = -1;

    int tot_grouped = 0;
    while (tot_grouped < position_count || col != 1) {  
        min_distance(dist_matrix, position_count, &index0, &index1);

        if (index0 == -1 || index1 == -1) {
            printf("No more edges found. Graph might be disconnected.\n");
            break; 
        }

        dist_matrix[index0][index1] = FLT_MAX;
        dist_matrix[index1][index0] = FLT_MAX;

        index_col0 = findIndex(tab, col, row, positions[index0]->index);
        index_col1 = findIndex(tab, col, row, positions[index1]->index);

        if (index_col0 == -1 && index_col1 == -1) {
            tab[col][row[col]] = index0;
            row[col]++;
            tab[col][row[col]] = index1;
            row[col]++;

            col++;
            tab = realloc(tab, (col+1) * sizeof(int*));
            tab[col] = (int*)malloc(2*sizeof(int));

            row = realloc(row, (col + 1) * sizeof(int));
            row[col] = 0;
        } else if (index_col0 != -1 && index_col1 == -1) {
            tab[index_col0] = realloc(tab[index_col0], (row[index_col0] + 1) * sizeof(int));
            tab[index_col0][row[index_col0]] = index1;
            row[index_col0]++;
        } else if (index_col0 == -1 && index_col1 != -1) {
            tab[index_col1] = realloc(tab[index_col1], (row[index_col1] + 1) * sizeof(int));
            tab[index_col1][row[index_col1]] = index0;
            row[index_col1]++;
        } 
        else if (index_col0 != index_col1) {
            if (index_col0 > index_col1) {
                int temp = index_col0;
                index_col0 = index_col1;
                index_col1 = temp;
            }
            
            tab[index_col0] = realloc(tab[index_col0], (row[index_col0] + row[index_col1]) * sizeof(int));
            
            for (int j = 0; j < row[index_col1]; j++) {
                tab[index_col0][row[index_col0]] = tab[index_col1][j];
                row[index_col0]++;
            }
            
            free(tab[index_col1]);

            for (int j = index_col1; j < col - 1; j++) {
                tab[j] = tab[j + 1];
                row[j] = row[j + 1];
            }
            
            col--;
            tab[col] = NULL;
            
            tab = realloc(tab, (col+1) * sizeof(int*));
            free(tab[col]);
            tab[col] = (int*)malloc(2*sizeof(int));
            row = realloc(row, (col + 1) * sizeof(int));
            row[col] = 0;
        }    
        tot_grouped = 0;  
        for (int i = 0; i < col; i++) {
            tot_grouped += row[i];
        }
    }

    long long sum = (long long)positions[index0]->x * (long long)positions[index1]->x;
    
    for (int i = 0; i <= col; i++) {
        if (tab[i] != NULL) {
            free(tab[i]);
            tab[i] = NULL;
        }
    }
    free(tab);
    tab = NULL;

    for (int i = 0; i < position_count; i++) {
        if (dist_matrix[i] != NULL) {
            free(dist_matrix[i]);
            dist_matrix[i] = NULL;
        }
    }
    free(dist_matrix);
    dist_matrix = NULL;

    free(row);
    row = NULL;
    
    return sum;

}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }
    clock_t start_time = clock();
    char line[1024];

    Position **positions = (Position **)malloc(1 * sizeof(Position*));

    int line_count = 0;
    int pos = 50;
    while (fgets(line, 1024, file)) {      
        char *ptr = line;
        Position *pos = (Position *)malloc(sizeof(Position));
        for (int i = 0; i < 3; i++) {
            char *end;
            long val = strtol(ptr, &end, 10);
            if (ptr == end) {
                fprintf(stderr, "Error parsing integer from line: %s\n", line);
                for (int j = 0; j < line_count; j++) free(positions[j]);
                free(positions);
                fclose(file);
                return 1;
            }
            ptr = end + 1;

            if (i == 0) pos->x = val;
            else if (i == 1) pos->y = val;
            else if (i == 2) pos->z = val;
        }
        pos->index = line_count;
        positions = realloc(positions, (line_count + 1) * sizeof(Position*));
        positions[line_count] = pos;
        line_count++;
    }

    long long sum = day8(positions, line_count);
    printf("Sum : %lld\n", sum);

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.6f seconds\n", time_taken);

    for (int i = 0; i < line_count; i++) {
        free(positions[i]);
        positions[i] = NULL;
    }
    free(positions);
    positions = NULL;
    fclose(file);
    free_memo();
    return 0;
}

