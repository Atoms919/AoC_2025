#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

short number_of_lines = 175;
const short max_number_of_lights = 10;
const short max_button_size = 20;

typedef struct {
    char *lights;
    int nb_of_switches;
} LightSwitch;

void show(char **goal, int goal_size, short ***buttons) {
    for (int i = 0; i < goal_size; i++) {
        printf("[", i);
        for (int j = 0; j < 10; j++) {
            printf("%c", goal[i][j]);
        }
        printf("] ");
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
        printf("\n");
    }
}

int are_lights_in_light_switches(char *lights, LightSwitch *light_switches, int light_switch_count) {
    for (int i = 0; i < light_switch_count; i++) {
        int all_match = 1;
        for (int j = 0; j < 10; j++) {
            if (light_switches[i].lights[j] != lights[j]) {
                all_match = 0;
                break;
            }
        }
        if (all_match) {
            return light_switches[i].nb_of_switches;
        }
    }
    return 0;
}

long Day10(char **goal, int goal_size, short ***buttons) {
    long result = 0;
    for (int i = 0; i < goal_size; i++) {
        char *lights = malloc(10 * sizeof(char));
        for (int i = 0; i < 10; i++) {
            lights[i] = '.';
        }
        int light_switch_size = 1024;
        LightSwitch *light_switches = malloc(light_switch_size * sizeof(LightSwitch));

        int light_switch_count = 1;
        light_switches[0].lights = lights;
        light_switches[0].nb_of_switches = 0;
        while(!are_lights_in_light_switches(goal[i], light_switches, light_switch_count)) {
            for (int j = 0; j < light_switch_count; j++) {
                for (int b = 0; b < max_button_size; b++) {
                    if (light_switch_count >= light_switch_size) 
                    {
                        light_switch_size *= 2;
                        light_switches = realloc(light_switches, light_switch_size * sizeof(LightSwitch));
                    }
                    if (buttons[i][b][0] == -1) break;
                    char *new_lights = malloc(10 * sizeof(char));
                    for (int k = 0; k < 10; k++) {
                        new_lights[k] = light_switches[j].lights[k];
                    }
                    for (int k = 0; k < max_number_of_lights; k++) {
                        if (buttons[i][b][k] == -1) break;
                        int light_index = buttons[i][b][k];
                        new_lights[light_index] = (new_lights[light_index] == '#') ? '.' : '#';
                    }
                    if (!are_lights_in_light_switches(new_lights, light_switches, light_switch_count)) {
                        light_switches[light_switch_count].lights = new_lights;
                        light_switches[light_switch_count].nb_of_switches = light_switches[j].nb_of_switches + 1;
                        light_switch_count++;
                    } else {
                        free(new_lights);
                    }
                }
            }
        }
        result += are_lights_in_light_switches(goal[i], light_switches, light_switch_count);
        for (int i = 0; i < light_switch_count; i++) {
            free(light_switches[i].lights);
            light_switches[i].lights = NULL;
        }
        free(light_switches);
        light_switches = NULL;
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

    char **goal = malloc(number_of_lines * sizeof(char *));
    int goal_size = 0;
    short *goal_size_line = malloc(number_of_lines * sizeof(short));
    for (int i = 0; i < number_of_lines; i++) {
        goal_size_line[i] = 0;
    }

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

    while (fgets(line, 1024, file)) {
        case_num = 0;
        int button_index = 0;
        //printf("Processing line %d: %s", count + 1, line);
        if (count >= number_of_lines) {
            printf("Reallocating for more lines\n");
            goal = realloc(goal, (count) * sizeof(char *));
            goal_size_line = realloc(goal_size_line, (count) * sizeof(short));
            buttons = realloc(buttons, (count) * sizeof(short **));
            buttons[count] = malloc(max_button_size * sizeof(short *));
            for (int j = 0; j < max_button_size; j++) {
                buttons[count][j] = malloc(max_number_of_lights * sizeof(short));
            }
            goal_size_line[count] = 0;
            number_of_lines++;
        }
        goal[count] = malloc(10 * sizeof(char));
        for (int i = 0; i < 10; i++) {
            goal[count][i] = '.';
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
                if (*p == '.' || *p == '#') {
                    goal[count - 1][goal_size_line[count - 1]++] = *p;  
                }
            }
            else if (case_num == 1) {
                //printf("Parsing button %d for goal %d\n", button_index, count - 1);
                int light_index = 0;
                while (*p != ')' && *p != ' ') 
                {
                    char *end;
                    int val = strtol(p, &end, 10);
                    if (end != p) {
                        //printf(" Parsed value: %d\n", val);
                        //printf(" Storing at buttons[%d][%d][%d]\n", count - 1, button_index, light_index);
                        buttons[count - 1][button_index][light_index] = val;
                        //printf(" Stored value: %d\n", buttons[count - 1][button_index][light_index]);
                        p = end - 1;
                        light_index++;
                    }
                    p++;
                }
                //printf(" Parsed %d lights for button %d\n", light_index, button_index);
                button_index++;
            }
            else if (case_num == 2) {
                //Extra data, ignored in part 1
                //printf("Parsing extra data for goal %d\n", count - 1);
            }
        }
    }

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
    free(goal_size_line);

    fclose(file);
    return 0;
}

