#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

short number_of_lines = 175;
const short max_number_of_lights = 10;
const short max_button_size = 20;

typedef struct {
    short *lights;
    int nb_of_clicks;
} LightClick;

void show(short **goal, int goal_size, short ***buttons) {
    for (int i = 0; i < goal_size; i++) {
        for (int j = 0; j < max_button_size; j++) {
            if(buttons[i][j][0] != -1) printf("(", j);
            for (int k = 0; k < max_number_of_lights; k++) {
                if (buttons[i][j][k] == -1) break;
                printf("%d", buttons[i][j][k]);
                if (buttons[i][j][k + 1] != -1) {
                    printf(",");
                }
            }
            if(buttons[i][j][0] != -1) printf(") ");
        }
        printf(" {");
        for (int j = 0; j < 10; j++) {
            printf("%d", goal[i][j]);
            if (j < 9) printf(",");
        }
        printf("} ");
        printf("\n");
    }
}

int are_lights_in_light_clicks(short *lights, LightClick *light_clicks, int light_click_count) {
    for (int i = 0; i < light_click_count; i++) {
        int all_match = 1;
        for (int j = 0; j < 10; j++) {
            if (light_clicks[i].lights[j] != lights[j]) {
                all_match = 0;
                break;
            }
        }
        if (all_match) {
            return light_clicks[i].nb_of_clicks;
        }
    }
    return 0;
}

long Day10(short **goal, int goal_size, short ***buttons) {
    long result = 0;
    for (int i = 0; i < goal_size; i++) {
        printf("Processing goal %d/%d: \n", i + 1, goal_size);
        short *lights = malloc(10 * sizeof(short));
        for (int i = 0; i < 10; i++) {
            lights[i] = 0;
        }
        int light_click_size = 1024;
        LightClick *light_clicks = malloc(light_click_size * sizeof(LightClick));

        int light_click_count = 1;
        light_clicks[0].lights = lights;
        light_clicks[0].nb_of_clicks = 0;

        while(!are_lights_in_light_clicks(goal[i], light_clicks, light_click_count)) {
            //printf("Processing goal %d: %d light configurations found\r", i + 1, light_click_count);
            int current_count = light_click_count;
            for (int j = 0; j < current_count; j++) {
                for (int b = 0; b < max_button_size; b++) {
                    printf("number of clicks: %d\r", light_clicks[current_count - 1].nb_of_clicks);
                    int stop = 0;
                    if (light_click_count >= light_click_size) 
                    {
                        light_click_size *= 2;
                        light_clicks = realloc(light_clicks, light_click_size * sizeof(LightClick));
                    }
                    if (buttons[i][b][0] == -1) break;
                    if (light_clicks[j].nb_of_clicks < light_clicks[current_count - 1].nb_of_clicks) {
                        break;
                    }
                    short *new_lights = malloc(10 * sizeof(short));
                    for (int k = 0; k < 10; k++) {
                        new_lights[k] = light_clicks[j].lights[k];
                    }
                    for (int k = 0; k < max_number_of_lights; k++) {
                        if (buttons[i][b][k] == -1) break;
                        int light_index = buttons[i][b][k];
                        new_lights[light_index] += 1;
                        if (new_lights[light_index] > goal[i][light_index]) {
                            stop = 1;
                            break;
                        }
                    }
                    if (stop == 1) {
                        free(new_lights);
                        continue;
                    }
                    if (!are_lights_in_light_clicks(new_lights, light_clicks, light_click_count)) {
                        light_clicks[light_click_count].lights = new_lights;
                        light_clicks[light_click_count].nb_of_clicks = light_clicks[j].nb_of_clicks + 1;
                        light_click_count++;
                    } else {
                        free(new_lights);
                    }
                }
            }
            
        }
        printf("Found light configuration for goal %d with %d clicks\n", i + 1, are_lights_in_light_clicks(goal[i], light_clicks, light_click_count));
        result += are_lights_in_light_clicks(goal[i], light_clicks, light_click_count);
        for (int i = 0; i < light_click_count; i++) {
            free(light_clicks[i].lights);
            light_clicks[i].lights = NULL;
        }
        free(light_clicks);
        light_clicks = NULL;
    }
    return result;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    int count = 0;
    int case_num = 0;

    short ***buttons = malloc(number_of_lines * sizeof(short **));
    for (int i = 0; i < number_of_lines; i++) {
        buttons[i] = malloc(max_button_size * sizeof(short *));
        for (int j = 0; j < max_button_size; j++) {
            buttons[i][j] = malloc(max_number_of_lights * sizeof(short));
            for (int k = 0; k < max_number_of_lights; k++) {
                buttons[i][j][k] = -1;
            }
        }
    }

    short **goal = malloc(number_of_lines * sizeof(short *));
    for (int i = 0; i < number_of_lines; i++) {
        goal[i] = malloc(max_number_of_lights * sizeof(short));
        for (int j = 0; j < max_number_of_lights; j++) {
            goal[i][j] = 0;
        }
    }

    while (fgets(line, 1024, file)) {
        case_num = 0;
        int button_index = 0;
        //printf("Processing line %d: %s", count + 1, line);
        if (count >= number_of_lines) {
            printf("Reallocating for more lines\n");
            buttons = realloc(buttons, (count) * sizeof(short **));
            buttons[count] = malloc(max_button_size * sizeof(short *));
            for (int j = 0; j < max_button_size; j++) {
                buttons[count][j] = malloc(max_number_of_lights * sizeof(short));
            }
            number_of_lines++;
        }
        count++;

        for (char *p = line; *p != '\0'; p++) {
            if (*p == ' ')
                continue;
            else if (*p == ']') {
                case_num++;
            }
            else if (*p == '{') {
                case_num++;
            }
            else if (case_num == 0)
            {
                //Ignored in part 2
            }
            else if (case_num == 1) {
                int light_index = 0;
                while (*p != ')' && *p != ' ') 
                {
                    char *end;
                    int val = strtol(p, &end, 10);
                    if (end != p) {
                        buttons[count - 1][button_index][light_index] = val;
                        p = end - 1;
                        light_index++;
                    }
                    p++;
                }
                button_index++;
            }
            else if (case_num == 2) {
                int goal_index = 0;
                while (*p != '}') 
                {
                    char *end;
                    int val = strtol(p, &end, 10);
                    if (end != p) {
                        goal[count - 1][goal_index++] = val;
                        p = end - 1;
                    }
                    p++;
                }
                break;
            }
        }
    }
    //printf("Parsed %d lines\n\n", count);
    //show(goal, count, buttons);

    long result = Day10(goal, count, buttons);
    printf("Result: %ld\n", result);

    // Free allocated memory
    for (int i = 0; i < count; i++) {
        free(goal[i]);
        for (int j = 0; j < max_button_size; j++) {
            free(buttons[i][j]);
        }
        free(buttons[i]);
    }
    free(goal);
    free(buttons);

    fclose(file);
    return 0;
}

