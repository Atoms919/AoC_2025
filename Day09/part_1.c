#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

typedef struct {   
    long x;
    long y;
} Position; 

long long day9(Position *positions, int count) {
    Position **top_left_corner = (Position **)malloc(100 * sizeof(Position *));
    int tlc_size = 100;
    top_left_corner[0] = &positions[0];
    int tlc_count = 1;
    Position **top_right_corner = (Position **)malloc(100 * sizeof(Position *));
    int trc_size = 100;
    top_right_corner[0] = &positions[0];
    int trc_count = 1;
    Position **bottom_left_corner = (Position **)malloc(100 * sizeof(Position *));
    int blc_size = 100;
    bottom_left_corner[0] = &positions[0];
    int blc_count = 1;
    Position **bottom_right_corner = (Position **)malloc(100 * sizeof(Position *));
    int brc_size = 100;
    bottom_right_corner[0] = &positions[0];
    int brc_count = 1;

    for (int i = 1; i < count; i++) {
        bool to_add = true;
        for (int j = 0; j < tlc_count; j++) {
            if ((positions[i].x <= top_left_corner[j]->x && positions[i].y < top_left_corner[j]->y) ||
                (positions[i].x < top_left_corner[j]->x && positions[i].y <= top_left_corner[j]->y)) {
                top_left_corner[j] = &positions[i];
                to_add = false;
                break;
            }
            else if (!(positions[i].x < top_left_corner[j]->x || positions[i].y < top_left_corner[j]->y)) {
                to_add = false;
            }
        }
        if (to_add) {
            if (tlc_count >= tlc_size) {
                tlc_size += 100;
                top_left_corner = realloc(top_left_corner, tlc_size * sizeof(Position *));
            }
            top_left_corner[tlc_count] = &positions[i];
            tlc_count++;
        }
        to_add = true;
        for (int j = 0; j < trc_count; j++) {
            if ((positions[i].x >= top_right_corner[j]->x && positions[i].y < top_right_corner[j]->y) ||
                (positions[i].x > top_right_corner[j]->x && positions[i].y <= top_right_corner[j]->y)) {
                top_right_corner[j] = &positions[i];
                to_add = false;
                break;
            }
            else if (!(positions[i].x > top_right_corner[j]->x || positions[i].y < top_right_corner[j]->y)) {
                to_add = false;
            }
        }
        if (to_add) {
            if (trc_count >= trc_size) {
                trc_size += 100;
                top_right_corner = realloc(top_right_corner, trc_size * sizeof(Position *));
            }
            top_right_corner[trc_count] = &positions[i];
            trc_count++;
        }
        to_add = true;
        for (int j = 0; j < blc_count; j++) {
            if ((positions[i].x <= bottom_left_corner[j]->x && positions[i].y > bottom_left_corner[j]->y) ||
                (positions[i].x < bottom_left_corner[j]->x && positions[i].y >= bottom_left_corner[j]->y)) {
                bottom_left_corner[j] = &positions[i];
                to_add = false;
                break;
            }
            else if (!(positions[i].x < bottom_left_corner[j]->x || positions[i].y > bottom_left_corner[j]->y)) {
                to_add = false;
            }
        }
        if (to_add) {
            if (blc_count >= blc_size) {
                blc_size += 100;
                bottom_left_corner = realloc(bottom_left_corner, blc_size * sizeof(Position *));
            }
            bottom_left_corner[blc_count] = &positions[i];
            blc_count++;
        }
        to_add = true;
        for (int j = 0; j < brc_count; j++) {
            if ((positions[i].x >= bottom_right_corner[j]->x && positions[i].y > bottom_right_corner[j]->y) ||
                (positions[i].x > bottom_right_corner[j]->x && positions[i].y >= bottom_right_corner[j]->y)) {
                bottom_right_corner[j] = &positions[i];
                to_add = false;
                break;
            }
            else if (!(positions[i].x > bottom_right_corner[j]->x || positions[i].y > bottom_right_corner[j]->y)) {
                to_add = false;
            }
        }
        if (to_add) {
            if (brc_count >= brc_size) {
                brc_size += 100;
                bottom_right_corner = realloc(bottom_right_corner, brc_size * sizeof(Position *));
            }
            bottom_right_corner[brc_count] = &positions[i];
            brc_count++;
        }
    }
    long long biggest_area = 0;
    for (int i = 0; i < tlc_count; i++) {
        for (int j = 0; j<brc_count; j++) {
            long long area = (llabs((long long)(bottom_right_corner[j]->x - top_left_corner[i]->x)) + 1) * (llabs((long long)(bottom_right_corner[j]->y - top_left_corner[i]->y)) + 1);
            if (area > biggest_area) {
                biggest_area = area;
            }
        }
    }
    for (int i = 0; i < trc_count; i++) {
        for (int j = 0;j<blc_count; j++) {
            long long area = (llabs((long long)(top_right_corner[i]->x - bottom_left_corner[j]->x)) + 1) * (llabs((long long)(bottom_left_corner[j]->y- top_right_corner[i]->y)) + 1);
            if (area > biggest_area) {
                biggest_area = area;
            }
        }
    }

    free(top_left_corner);
    free(top_right_corner);
    free(bottom_left_corner);
    free(bottom_right_corner);

    return biggest_area;
}
int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];

    clock_t start_time = clock();

    Position *positions = (Position *)malloc(500 * sizeof(Position));
    int size = 500;

    int line_count = 0;
    while (fgets(line, 1024, file)) {      
        char *ptr = line;
        Position pos = {0, 0};
        for (int i = 0; i < 2; i++) {
            char *end;
            long val = strtol(ptr, &end, 10);
            if (ptr == end) {
                fprintf(stderr, "Error parsing integer from line: %s\n", line);
                free(positions);
                fclose(file);
                return 1;
            }
            ptr = end + 1;

            if (i == 0) pos.x = val;
            else if (i == 1) pos.y = val;
        }
        line_count++;
        if (line_count>=size) {
            positions = realloc(positions, (size+500) * sizeof(Position));
            size += 500;
        }
        positions[line_count - 1] = pos;
    }

    long long biggest_area = day9(positions, line_count);
    printf("Biggest Area : %lld\n", biggest_area);

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %.6f seconds\n", time_spent);

    free(positions);
    fclose(file);

    return 0;
}

