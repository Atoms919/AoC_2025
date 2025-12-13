#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <float.h>

typedef struct {   
    long x;
    long y;
    long z;
    float distance_min;
    int index;
} Position; 


void afficher(Position **positions, int position_count) {
    printf("\nPositions:\n");
    for (int i = 0; i < position_count; i++) {
        printf("Position %d: indexe %d (%ld, %ld, %ld) - Min Distance: %f\n", i, positions[i]->index, positions[i]->x, positions[i]->y, positions[i]->z, positions[i]->distance_min);
    }
}

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

void min_distance(Position **pos, int position_count) {
    for(int i = 0; i < position_count; i++) {
        for (int j = i+1; j < position_count; j++) {
            float dx = pos[i]->x - pos[j]->x;
            float dy = pos[i]->y - pos[j]->y;
            float dz = pos[i]->z - pos[j]->z;
            float distance = sqrt((float)(dx*dx + dy*dy + dz*dz));
            if (distance < pos[i]->distance_min) {
                pos[i]->distance_min = distance;
            }
            if (distance < pos[j]->distance_min) {
                pos[j]->distance_min = distance;
            }
        }
    }
}

void min_distance_single(Position **pos, int position_count, float min_distance, int start_index) {
    pos[0]->distance_min = FLT_MAX;
    for(int i = start_index; i < position_count; i++) {
        float dx = pos[0]->x - pos[i]->x;
        float dy = pos[0]->y - pos[i]->y;
        float dz = pos[0]->z - pos[i]->z;
        float distance = sqrt((float)(dx*dx + dy*dy + dz*dz));
        if (distance < pos[0]->distance_min && (distance > min_distance || (i == start_index && distance == pos[i]->distance_min))) {
            pos[0]->distance_min = distance;
            //printf("New min distance for (%ld, %ld, %ld): %f (to (%ld, %ld, %ld))\n", pos[0]->x, pos[0]->y, pos[0]->z, pos[0]->distance_min, pos[i]->x, pos[i]->y, pos[i]->z);
        }
    }
    //printf("After search: pos[0] = (%ld, %ld, %ld), distance_min = %f\n", pos[0]->x, pos[0]->y, pos[0]->z, pos[0]->distance_min);
    order_positions_by_distance(pos, position_count);
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

long day8(Position **positions, int position_count) {
    int** tab = (int**)malloc(sizeof(int*));
    tab[0] = (int*)malloc(2*sizeof(int));
    int col = 0;
    int *row = (int*)malloc(sizeof(int));
    row[0] = 0;

    int count = 0;
    while (count < 1000) {
        printf("Iteration %d/1000\n", count + 1);    

        int indexe0 = positions[0]->index;
        int indexe1 = positions[1]->index;

        int index_col0 = findIndex(tab, col, row, indexe0);
        int index_col1 = findIndex(tab, col, row, indexe1);

        if (index_col0 == -1 && index_col1 == -1) {
            //printf("Creating new group for (%d, %d)\n", indexe0, indexe1);
            tab[col][row[col]] = indexe0;
            row[col]++;
            tab[col][row[col]] = indexe1;
            row[col]++;

            col++;
            tab = realloc(tab, (col+1) * sizeof(int*));
            tab[col] = (int*)malloc(2*sizeof(int));

            row = realloc(row, (col + 1) * sizeof(int));
            row[col] = 0;
        } else if (index_col0 != -1 && index_col1 == -1) {
            //printf("Adding (1) %d to group %d\n", indexe1, index_col0);
            tab[index_col0] = realloc(tab[index_col0], (row[index_col0] + 1) * sizeof(int));
            tab[index_col0][row[index_col0]] = indexe1;
            row[index_col0]++;
        } else if (index_col0 == -1 && index_col1 != -1) {
            //printf("Adding (2) %d to group %d\n", indexe0, index_col1);
            tab[index_col1] = realloc(tab[index_col1], (row[index_col1] + 1) * sizeof(int));
            tab[index_col1][row[index_col1]] = indexe0;
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
        

        min_distance_single(positions, position_count, positions[0]->distance_min, 2);
        min_distance_single(positions, position_count, positions[0]->distance_min, 1);

        //afficher(positions, position_count);
        if (positions[0]->distance_min == FLT_MAX) {
            printf("No more valid pairs found at iteration %d\n", count);
            break;
        }

        count++;
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

    min_distance(positions, line_count);
    order_positions_by_distance(positions, line_count);
    //afficher(positions, line_count);
    long sum = day8(positions, line_count);

    for (int i = 0; i < line_count; i++) free(positions[i]);
    free(positions);
    fclose(file);

    printf("Total : %ld\n", sum);
    return 0;
}

