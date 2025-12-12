#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

typedef struct {
    char **grid;
    int area;
} shape;

typedef struct {
    int length;
    int width;
    int *shapes_to_fit;
} space;

void print_shapes(shape *shapes, int shape_count) {
    printf("\nTotal Shapes: %d\n", shape_count);
    for (int i = 0; i < shape_count; i++) {
        printf("Shape %d (Area: %d):\n", i , shapes[i].area);
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                printf("%c", shapes[i].grid[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void print_spaces(space *spaces, int space_count, int shape_count) {
    printf("\nTotal Spaces: %d\n", space_count);
    for (int i = 0; i < space_count; i++) {
        printf("Space %d (Length: %d, Width: %d):\n", i, spaces[i].length, spaces[i].width);
        printf("Shapes to fit:\n");
        for (int j = 0; j < shape_count; j++) {
            printf("Shape %d: %d\n", j, spaces[i].shapes_to_fit[j]);
        }
        printf("\n");
    }
}

int check_area_fit_1(space space) {
    int space_area = ((int)(space.length/3) * (int)(space.width/3)) * 9;
    int total_shape_area = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < space.shapes_to_fit[i]; j++) {
            total_shape_area += 9; 
        }
    }
    return total_shape_area <= space_area;
}

int check_area_fit_2(space space, shape *shapes, int shape_count) {
    int space_area = space.length * space.width;
    int total_shape_area = 0;
    for (int i = 0; i < shape_count; i++) {
        total_shape_area += shapes[i].area * space.shapes_to_fit[i];
    }
    return total_shape_area <= space_area;
}

void day12(space *spaces, int space_count, shape *shapes, int shape_count) {
    int sum = 0;
    int sum_unknown = 0;
    for (int i = 0; i < space_count; i++) {
        if(check_area_fit_1(spaces[i])) {
            sum++;
        }
        else if(check_area_fit_2(spaces[i], shapes, shape_count)) {
            sum_unknown++;
        }
    }
    printf("Count that definitely fit: %d\n", sum);
    printf("Count that definitely don't fit: %d\n", space_count - sum - sum_unknown);
    printf("Count that we don't know if they fit yet: %d\n", sum_unknown);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    int count = 0;
    int shape_row = 0;
    shape *shapes = malloc(6 * sizeof(shape));
    int shape_count = 0;
    space *spaces = malloc(0);
    int space_count = 0;

    while (fgets(line, 1024, file)) {
        if (count == 0) {
            count++;
            continue;
        }
        char *p = line;
        char *end;
        int val = strtol(p, &end, 10);
        if (*(p+1) == ':') {
            continue;
        }
        else if (p == end) {
            if (shape_row == 0) {
                if (shape_count > 5) shapes = realloc(shapes, shape_count * sizeof(shape));
                shapes[shape_count].area = 0;
                shape_count++;
                shapes[shape_count - 1].grid = malloc(3 * sizeof(char *));
            }
            if (shape_row >= 3) 
            {
                shape_row = 0;
            }
            else{
                shapes[shape_count - 1].grid[shape_row] = malloc(3 * sizeof(char));
                for(int i = 0; i < 3; i++) {
                    shapes[shape_count - 1].grid[shape_row][i] = *p;
                    if (*p == '#') shapes[shape_count - 1].area++;
                    p++;
                }
                shape_row++;
            }            
        }
        else {
            // Process space dimensions  
            int length = strtol(p, &end, 10);
            p = end+1;
            int width = strtol(p, &end, 10);
            p = end+1;
            space_count++;
            spaces = realloc(spaces, space_count * sizeof(space));
            spaces[space_count - 1].length = length;
            spaces[space_count - 1].width = width;
            spaces[space_count - 1].shapes_to_fit = malloc(shape_count * sizeof(int));
            for (int i = 0; i < shape_count; i++) {
                int number_to_fit = strtol(p, &end, 10);
                p = end;
                spaces[space_count - 1].shapes_to_fit[i] = number_to_fit;
            }
        }
    }

    //print_shapes(shapes, shape_count);
    //print_spaces(spaces, space_count, shape_count);
    day12(spaces, space_count, shapes, shape_count);

    fclose(file);
    return 0;
}

 