#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

typedef struct {   
    long x;
    long y;
} Position; 

const int max = 100000;

int is_point_in_polygon(Position point, char **polygon) {
    int inside = 0;

    if (polygon[point.y][point.x] == 'X' || polygon[point.y][point.x] == 'O') {
        return 1;
    }
    
    long x = point.x;
    long y = point.y;
    
    long dist_x_max = max - x;
    long dist_y_max = max - y;

    int count_edges = 0;
    int count_corners = 0;
    if (x == fmin(fmin(x, y), fmin(dist_x_max, dist_y_max))) {
        for (int i = 0; i < x; i++) {
            if (polygon[y][i] == 'O') {
                if (count_corners == 0) {
                    count_corners = 1;
                    count_edges++;
                }
                else {
                    count_corners = 0;
                }
            }
            else if (count_corners == 0 && polygon[y][i] == 'X') {
                count_edges++;
            }
        }
    }
    else if (y == fmin(fmin(x, y), fmin(dist_x_max, dist_y_max))) {
        for (int i = 0; i < y; i++) {
            if (polygon[i][x] == 'O') {
                if (count_corners == 0) {
                    count_corners = 1;
                    count_edges++;
                }
                else {
                    count_corners = 0;
                }
            }
            else if (count_corners == 0 && polygon[i][x] == 'X') {
                count_edges++;
            }
        }
    }
    else if (dist_x_max == fmin(fmin(x, y), fmin(dist_x_max, dist_y_max))) {
        for (int i = max-1; i > x; i--) {
            if (polygon[y][i] == 'O') {
                if (count_corners == 0) {
                    count_corners = 1;
                    count_edges++;
                }
                else {
                    count_corners = 0;
                }
            }
            else if (count_corners == 0 && polygon[y][i] == 'X') {
                count_edges++;
            }
        }
    }
    else if (dist_y_max == fmin(fmin(x, y), fmin(dist_x_max, dist_y_max))) {
        for (int i = max-1; i > y; i--) {
            if (polygon[i][x] == 'O') {
                if (count_corners == 0) {
                    count_corners = 1;
                    count_edges++;
                }
                else {
                    count_corners = 0;
                }
            }
            else if (count_corners == 0 && polygon[i][x] == 'X') {
                count_edges++;
            }
        }
    }

    if (count_edges % 2 == 1) {
        inside = 1;
    }
    else {
        inside = 0;
    }

    return inside;
}
int is_horizontal_edge_inside_polygon(Position cornerA, Position cornerB, char **polygon) {   
    if (cornerA.y != cornerB.y) {
        fprintf(stderr, "Error: is_horizontal_edge_inside_polygon called with non-horizontal edge (%ld,%ld)-(%ld,%ld)\n", cornerA.x, cornerA.y, cornerB.x, cornerB.y);
        return 0;
    }

    long xA = cornerA.x;
    long xB = cornerB.x;

    if (cornerA.x > cornerB.x) {
        xA = cornerB.x;
        xB = cornerA.x;
    }   

    long dist_xB_max = max - xB;

    int count_edges = 0;
    int count_corners = 0;
    char previous_char = 'X';
    char c = polygon[cornerA.y][xA];
    char next_char = polygon[cornerA.y][xA+1];
    for (int i = xA; i < xB - 1; i++) {
        if (previous_char == '.' && c != '.' && next_char == '.'){
            return 0;
        }
        previous_char = c;
        c = next_char;
        next_char = polygon[cornerA.y][i+2];
    }
    
    return 1;
}
int is_vertical_edge_inside_polygon(Position cornerA, Position cornerB, char **polygon) {
    if (cornerA.x != cornerB.x) {
        fprintf(stderr, "Error: is_vertical_edge_inside_polygon called with non-vertical edge (%ld,%ld)-(%ld,%ld)\n", cornerA.x, cornerA.y, cornerB.x, cornerB.y);
        return 0;
    }

    long yA = cornerA.y;
    long yB = cornerB.y;

    if (cornerA.y > cornerB.y) {
        yA = cornerB.y;
        yB = cornerA.y;
    }

    long dist_yB_max = max - yB;

    int count_edges = 0;
    int count_corners = 0;
    char previous_char = 'X';
    char c = polygon[yA][cornerA.x];
    char next_char = polygon[yA+1][cornerA.x];
    for (int i = yA; i < yB - 1; i++) {
        if (previous_char == '.' && c != '.' && next_char == '.') {
            return 0;
        }
        previous_char = c;
        c = next_char;
        next_char = polygon[i+2][cornerA.x];
    }
    return 1;
}

int is_rectangle_inside_polygon(Position cornerA, Position cornerB, char **polygon) {
    Position cornerC = {cornerA.x, cornerB.y};
    Position cornerD = {cornerB.x, cornerA.y};

    if (!is_point_in_polygon(cornerC, polygon) || !is_point_in_polygon(cornerD, polygon)) {
        return 0;
    }

    if(!is_vertical_edge_inside_polygon(cornerB, cornerD, polygon)) return 0;
    if(!is_vertical_edge_inside_polygon(cornerC, cornerA, polygon)) return 0;
    if(!is_horizontal_edge_inside_polygon(cornerB, cornerC, polygon)) return 0;
    if(!is_horizontal_edge_inside_polygon(cornerA, cornerD, polygon)) return 0;
    
    return 1;
}

long long day9(Position *positions, int count) {
    char **pos = (char **)malloc(max * sizeof(char *));
    for (int i = 0; i < max; i++) {
        pos[i] = (char *)malloc(max * sizeof(char));
        for (int j = 0; j < max; j++) {
            pos[i][j] = '.';
        }
    }

    for (int i = 0; i < count; i++) {
        Position *pointA = &positions[count - 1];
        Position *pointB = &positions[i];
        if (i != 0) {
            pointA = &positions[i - 1];
        }
        if (pointA->x == pointB->x) {
            for (long y = fmin(pointA->y, pointB->y); y <= fmax(pointA->y, pointB->y); y++) {
                pos[y][pointA->x] = 'X';
            }
        }
        else if (pointA->y == pointB->y) {
            for (long x = fmin(pointA->x, pointB->x); x <= fmax(pointA->x, pointB->x); x++) {
                pos[pointA->y][x] = 'X';
            }
        }
        pos[pointB->y][pointB->x] = 'O';
        pos[pointA->y][pointA->x] = 'O';
    }

    long long biggest_area = 0;
    long long counter = 0;
    for (int i = 0; i < count; i++) {
        Position cornerA = positions[i];
        long long area = 0;
        for (int j = i; j < count; j++) {
            counter++;
            printf("%lld\r", counter);
            Position cornerB = positions[j];
            long long length = llabs(cornerB.x - cornerA.x) + 1;
            long long width = llabs(cornerB.y - cornerA.y) + 1;
            area = length * width;
            //printf("\nChecking rectangle: A(%ld,%ld) B(%ld,%ld) Area: %lld\n", cornerA.x, cornerA.y, cornerB.x, cornerB.y, area);
            if (area > biggest_area) {
                if (is_rectangle_inside_polygon(cornerA, cornerB, pos)) {
                    biggest_area = area;
                    printf("Rectangle A(%ld,%ld) B(%ld,%ld) is inside polygon. New biggest area: %lld\n", cornerA.x, cornerA.y, cornerB.x, cornerB.y, biggest_area);
                }
            }
        }
    }

    for (int i = 0; i < max; i++) {
        free(pos[i]);
    }
    free(pos);

    return biggest_area;
}
int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];

    LARGE_INTEGER frequency, start_time, end_time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start_time);

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

    QueryPerformanceCounter(&end_time);
    double elapsed_time = (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
    printf("Time taken: %.6f seconds\n", elapsed_time);
    
    free(positions);
    fclose(file);

    return 0;
}

