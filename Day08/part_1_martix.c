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
    float distance_min;
    int index;
} Position; 

typedef struct {
    int index_a;
    int index_b;
} IndexPair;

void order_positions_by_distance(Position **positions, int position_count) {
    for (int i = 0; i < position_count - 1; i++) {
        for (int j = 0; j < position_count - i - 1; j++) {
            if (positions[j]->distance_min > positions[j + 1]->distance_min) {
                Position *temp = positions[j];
                positions[j] = positions[j + 1];
                positions[j + 1] = temp;
            }
        }
    }
}

IndexPair min_distance(float** dist_matrix, int position_count) {
    float min_dist = FLT_MAX;
    int index_a = -1;
    int index_b = -1;

    for (int i = 0; i < position_count; i++) {
        for (int j = 0; j < position_count; j++) {
            if (i != j && dist_matrix[i][j] < min_dist) {
                min_dist = dist_matrix[i][j];
                index_a = i;
                index_b = j;
            }
        }
    }
    dist_matrix[index_a][index_b] = FLT_MAX;
    dist_matrix[index_b][index_a] = FLT_MAX;
    IndexPair result = {index_a, index_b};
    return result;
}

float** all_distances(Position **pos, int position_count) {
    float **dist_matrix = (float **)malloc(position_count * sizeof(float*));
    for (int i = 0; i < position_count; i++) {
        dist_matrix[i] = (float *)malloc(position_count * sizeof(float));
        for (int j = 0; j < position_count; j++) {
            if (i == j) {
                dist_matrix[i][j] = FLT_MAX;
            } else {
                float dx = pos[i]->x - pos[j]->x;
                float dy = pos[i]->y - pos[j]->y;
                float dz = pos[i]->z - pos[j]->z;
                dist_matrix[i][j] = sqrt((float)(dx*dx + dy*dy + dz*dz));
            }
        }
    }
    return dist_matrix;
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

long day8(Position **positions, int position_count) {
    int** tab = (int**)malloc(sizeof(int*));
    tab[0] = (int*)malloc(2*sizeof(int));
    int col = 0;
    int *row = (int*)malloc(sizeof(int));
    row[0] = 0;

    float** dist_matrix = all_distances(positions, position_count);

    for (int _ = 0; _ < 1000; _++) {
        //printf("Iteration %d/1000\n", _);
        IndexPair pair = min_distance(dist_matrix, position_count);
        int index0 = pair.index_a;
        int index1 = pair.index_b;

        if (index0 == -1 || index1 == -1) {
            break; // No more pairs found
        }

        int index_col0 = findIndex(tab, col, row, positions[index0]->index);
        int index_col1 = findIndex(tab, col, row, positions[index1]->index);

        if (index_col0 == -1 && index_col1 == -1) {
            //printf("Creating new group for (%d, %d)\n", index0, index1);
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
            //printf("Adding (1) %d to group %d\n", index1, index_col0);
            tab[index_col0] = realloc(tab[index_col0], (row[index_col0] + 1) * sizeof(int));
            tab[index_col0][row[index_col0]] = index1;
            row[index_col0]++;
        } else if (index_col0 == -1 && index_col1 != -1) {
            //printf("Adding (2) %d to group %d\n", index0, index_col1);
            tab[index_col1] = realloc(tab[index_col1], (row[index_col1] + 1) * sizeof(int));
            tab[index_col1][row[index_col1]] = index0;
            row[index_col1]++;
        } 
        else if (index_col0 != index_col1) {
            //printf("Merging groups %d and %d\n", index_col0, index_col1);
            
            if (index_col0 > index_col1) {
                int temp = index_col0;
                index_col0 = index_col1;
                index_col1 = temp;
            }
            
            tab[index_col0] = realloc(tab[index_col0], (row[index_col0] + row[index_col1]) * sizeof(int));
            //Merge columns
            for (int j = 0; j < row[index_col1]; j++) {
                tab[index_col0][row[index_col0]] = tab[index_col1][j];
                row[index_col0]++;
            }
            
            //printf("group %d being freed (Ptr: %p): ", index_col1, (void*)tab[index_col1]);
            for (int j = 0; j < row[index_col1]; j++) {
                //printf("%d ", tab[index_col1][j]);
            }
            //printf("\n");
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
        else {
            // Both indices are already in the same group; do nothing
            //printf("Both indices are already in the same group; no action taken.\n");
        }
    }

    order_groups_by_size(tab, col, row);

    long sum = 1;

    for (int i = 0; i < 3 && i < col; i++) {
        sum *= row[i];
    }
   
    //printf("Final groups:\n");
    for (int i = 0; i < col; i++) {
        //printf("Group %d (Ptr: %p): ", i, (void*)tab[i]);
        for (int j = 0; j < row[i]; j++) {
            //printf("%d ", tab[i][j]);
        }
        free(tab[i]);
        //printf("\t(%d members)\n", row[i]);
    }

    free(tab);
    free(row);
    
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
        pos->distance_min = FLT_MAX;
        pos->index = line_count;
        positions = realloc(positions, (line_count + 1) * sizeof(Position*));
        positions[line_count] = pos;
        line_count++;
    }

    long sum = day8(positions, line_count);

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %.6f seconds\n", time_spent);

    for (int i = 0; i < line_count; i++) free(positions[i]);
    free(positions);
    fclose(file);

    printf("Total : %ld\n", sum);
    return 0;
}

