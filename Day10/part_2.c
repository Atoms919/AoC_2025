#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

const int MAX_LIGHTS = 10;
const int MAX_BUTTONS = 20;

int solution_buffer[10000]; 
int solution_count_global = 0;

void find_combinations(int current_lights, int goal, int *button_masks, int button_idx, int current_press_mask, int max_buttons) {
    if (button_idx == max_buttons) {
        if (current_lights == goal) {
            solution_buffer[solution_count_global++] = current_press_mask;
        }
        return;
    }
    find_combinations(current_lights, goal, button_masks, button_idx + 1, current_press_mask, max_buttons);
    if (button_masks[button_idx] != -1) {
        find_combinations(current_lights ^ button_masks[button_idx], goal, button_masks, button_idx + 1, current_press_mask | (1 << button_idx), max_buttons);
    }
}

int count_set_bits(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1);
        count++;
    }
    return count;
}

int solve_recursive(int *goal_state, int *button_masks, int num_buttons) {
    int all_zero = 1;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (goal_state[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    if (all_zero) return 0;

    int current_goal_mask = 0;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (goal_state[i] % 2 != 0) {
            current_goal_mask |= (1 << i);
        }
    }
    solution_count_global = 0;
    find_combinations(0, current_goal_mask, button_masks, 0, 0, num_buttons);

    if (solution_count_global == 0) return 100000;

    int min_total_presses = 100000;
    int local_solution_count = solution_count_global;
    int local_solutions[1024]; 
    for(int i=0; i<local_solution_count; i++) local_solutions[i] = solution_buffer[i];

    int next_goal[MAX_LIGHTS];
    
    for (int i = 0; i < local_solution_count; i++) {
        int pressed_mask = local_solutions[i];
        int pressed_count = count_set_bits(pressed_mask);

        for(int k=0; k<MAX_LIGHTS; k++) next_goal[k] = goal_state[k];

        for (int b = 0; b < num_buttons; b++) {
            if ((pressed_mask >> b) & 1) {
                int b_mask = button_masks[b];
                for (int l = 0; l < MAX_LIGHTS; l++) {
                    if ((b_mask >> l) & 1) {
                        next_goal[l]--;
                    }
                }
            }
        }

        int valid = 1;
        for (int l = 0; l < MAX_LIGHTS; l++) {
            if (next_goal[l] < 0 || next_goal[l] % 2 != 0) {
                valid = 0;
                break;
            }
            next_goal[l] /= 2; 
        }

        if (valid) {
            int res = solve_recursive(next_goal, button_masks, num_buttons);
            if (res < 100000) {
                int total = res * 2 + pressed_count;
                if (total < min_total_presses) {
                    min_total_presses = total;
                }
            }
        }
    }

    return min_total_presses;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) return 1;

    int all_buttons[200][20];
    int all_goals[200][10];  
    int all_num_buttons[200];
    int line_count = 0;

    char line[1024];
    while (fgets(line, 1024, file)) {
        int case_num = 0;
        int btn_idx = 0;
        int light_idx = 0;
        
        for(int b=0; b<MAX_BUTTONS; b++) all_buttons[line_count][b] = 0;
        all_num_buttons[line_count] = 0;

        char *p = line;
        while (*p) {
            if (*p == '{') { case_num = 2; light_idx = 0; p++; continue; }
            if (*p == '}') { break; }
            
            if (case_num == 0 && *p == ']') { case_num = 1; p++; continue; } 
            if (case_num == 1) { 
                if (*p == '(') {
                    p++;
                    while (*p != ')') {
                        if (*p >= '0' && *p <= '9') {
                            int l = strtol(p, &p, 10);
                            all_buttons[line_count][btn_idx] |= (1 << l);
                        } else {
                            p++;
                        }
                    }
                    btn_idx++;
                    all_num_buttons[line_count] = btn_idx;
                }
            }
            else if (case_num == 2) { 
                if (*p >= '0' && *p <= '9') {
                    all_goals[line_count][light_idx++] = strtol(p, &p, 10);
                }
            }
            p++;
        }
        line_count++;
    }
    fclose(file);

    clock_t start_time = clock();
    long total_result = 0;

    for (int i = 0; i < line_count; i++) {
        int res = solve_recursive(all_goals[i], all_buttons[i], all_num_buttons[i]);
        // printf("Result %d: %d\n", i+1, res);
        total_result += res;
    }

    clock_t end_time = clock();
    printf("Total Result: %ld\n", total_result);
    printf("Time: %.6f s\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}
