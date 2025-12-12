#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

short number_of_lines = 175;
const short max_number_of_lights = 10;
const short max_number_of_buttons = 20;

typedef struct {
    char *lights;
    int nb_of_switches;
} LightSwitch;

void show(short **goal, int goal_size, short ***buttons) {
    for (int i = 0; i < goal_size; i++) {
        for (int j = 0; j < max_number_of_buttons; j++) {
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

int print_solutions(int **solution, int solution_count) {
    for (int i = 0; i < solution_count; i++) {
        printf("Solution %d: ", i + 1);
        for (int j = 0; j < max_number_of_buttons; j++) {
            if (solution[i][j]) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
    return 0;
}

int goal_achieved(char *lights, char *goal) {
    for (int i = 0; i < max_number_of_lights; i++) {
        if (lights[i] != goal[i]) {
            return 0;
        }
    }
    return 1;
}

void part_1(char *lights, char *goal, short **buttons, int button_index, int *current_solution, int ***all_solutions, int *solution_count) {
    //Implement the same logic as in part 1 to find all possible solutions
    //Return an array of solutions (each solution is an array of button indices)
    if (goal_achieved(lights, goal)) {
        //Found a solution
        (*all_solutions) = realloc((*all_solutions), ((*solution_count) + 1) * sizeof(int *));
        (*all_solutions)[*solution_count] = malloc(max_number_of_buttons * sizeof(int));
        for (int i = 0; i < max_number_of_buttons; i++) {
            (*all_solutions)[*solution_count][i] = current_solution[i];
        }
        (*solution_count)++;
    }
    else if (button_index >= max_number_of_buttons) {
        //not a solution
        return;
    }
    else {
        //Try without pressing the button
        part_1(lights, goal, buttons, button_index + 1, current_solution, all_solutions, solution_count);

        //Try pressing the button
        //Toggle the lights
        for (int i = 0; i < max_number_of_lights; i++) {
            if (buttons[button_index][i] == -1) break;
            int light_index = buttons[button_index][i];
            lights[light_index] = (lights[light_index] == '.') ? '#' : '.';
        }
        current_solution[button_index] = 1;
        part_1(lights, goal, buttons, button_index + 1, current_solution, all_solutions, solution_count);
        current_solution[button_index] = 0;
        //Toggle back the lights
        for (int i = 0; i < max_number_of_lights; i++) {
            if (buttons[button_index][i] == -1) break;
            int light_index = buttons[button_index][i];
            lights[light_index] = (lights[light_index] == '.') ? '#' : '.';
        }
    }
}

int goal_is_zero(short *goal) {
    for (int i = 0; i < max_number_of_lights; i++) {
        if (goal[i] != 0) {
            return 0;
        }
    }
    return 1;
}

int solve(short *goal, short **buttons, int button_count) {
    if (goal_is_zero(goal)) {
        return 0;
    }

    char *temp_goal = malloc(max_number_of_lights * sizeof(short));
    for (int i = 0; i < max_number_of_lights; i++) {
        temp_goal[i] = (goal[i]%2==0) ? '.':'#';
    }
    //the problem is now the same as part 1 with temp_goal
    int **solutions = malloc(0 * sizeof(int *));
    int solution_count = 0;
    int *current_solution = calloc(max_number_of_buttons, sizeof(int));
    char *lights = malloc(max_number_of_lights * sizeof(char));
    for (int i = 0; i < max_number_of_lights; i++) {
        lights[i] = '.';
    }
    //Find all solutions
    part_1(lights, temp_goal, buttons, 0, current_solution, &solutions, &solution_count);
    //print_solutions(solutions, solution_count);

    int *results = malloc(solution_count * sizeof(int));

    for(int i=0; i<solution_count; i++) {
        short *new_goal = malloc(max_number_of_lights * sizeof(short));
        for (int j = 0; j < max_number_of_lights; j++) {
            new_goal[j] = goal[j];
        }
        //Update new_goal based on solution
        int number_of_pressed_buttons = 0;
        for (int j = 0; j < max_number_of_buttons; j++) {
            if (solutions[i][j]) {
                for (int k = 0; k < max_number_of_lights; k++) {
                    if (buttons[j][k] == -1) break;
                    int light_index = buttons[j][k];
                    new_goal[light_index]--;
                }
                number_of_pressed_buttons++;
            }
        }
        //check that new_goal is valid (all values >=0)
        int valid = 1;
        for (int j = 0; j < max_number_of_lights; j++)
        {
            if (new_goal[j] < 0) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            results[i] = 1000;
            free(new_goal);
            continue;
        }
        //Divide new_goal by 2
        for (int j = 0; j < max_number_of_lights; j++) {
            new_goal[j] /= 2;
        }
        int sol = solve(new_goal, buttons, button_count);
        if (sol == 1000) {
            results[i] = 1000;
            free(new_goal);
            continue;
        }
        results[i] = sol * 2 + number_of_pressed_buttons;
        //printf("Result for solution %d: %d\n", i + 1, results[i]);
        free(new_goal);
    }
    //printf("All results computed\n\n");
    //Find minimum result
    int min_result = 1000;
    for (int i = 0; i < solution_count; i++) {
        if (results[i] < min_result) {
            min_result = results[i];
        }
    }
    free(results);
    return min_result;
}

long Day10(short **goal, int goal_size, short ***buttons) {
    long result = 0;
    for (int i = 0; i < goal_size; i++) {
        int res = solve(goal[i], buttons[i], max_number_of_buttons);
        printf("Result for goal %d: %d\n", i + 1, res);
        result += res;
    }
    return result;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    clock_t start_time = clock();

    char line[1024];
    int count = 0;
    int case_num = 0;

    short ***buttons = malloc(number_of_lines * sizeof(short **));
    for (int i = 0; i < number_of_lines; i++) {
        buttons[i] = malloc(max_number_of_buttons * sizeof(short *));
        for (int j = 0; j < max_number_of_buttons; j++) {
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
            buttons[count] = malloc(max_number_of_buttons * sizeof(short *));
            for (int j = 0; j < max_number_of_buttons; j++) {
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
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken: %.6f seconds\n", time_spent);
    // Free allocated memory
    for (int i = 0; i < count; i++) {
        free(goal[i]);
        for (int j = 0; j < max_number_of_buttons; j++) {
            free(buttons[i][j]);
        }
        free(buttons[i]);
    }
    free(goal);
    free(buttons);

    fclose(file);
    return 0;
}

