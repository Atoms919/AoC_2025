#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

short number_of_lines = 175;
const short max_number_of_lights = 10;
const short max_button_size = 20;

long global_min_clicks = LONG_MAX;

// Structure to hold the system of equations
typedef struct {
    double coeffs[20];
    double constant;
} Equation;

// Context to pass data to the solver helper
typedef struct {
    Equation *system;
    int *pivot_col;
    int pivot_row_count;
    int *free_indices;
    int free_count;
    int search_limit;
    int *free_vals;
} SolverContext;

// Helper function to check a specific configuration of free variables
void check_configuration(SolverContext *ctx) {
    double solution[20];
    
    // 1. Set Free Variables
    for(int i = 0; i < ctx->free_count; i++) {
        solution[ctx->free_indices[i]] = (double)ctx->free_vals[i];
    }
    
    long current_sum = 0;
    for(int i = 0; i < ctx->free_count; i++) {
        current_sum += ctx->free_vals[i];
    }

    // 2. Back-Substitute to find Pivot Variables
    // The system is in RREF. Row 'r' has the pivot for variable 'pivot_col[r]'.
    // Equation: 1.0 * pivot + sum(coeff * free) = constant
    // Pivot = constant - sum(coeff * free)
    for (int r = 0; r < ctx->pivot_row_count; r++) {
        int p_idx = ctx->pivot_col[r];
        double val = ctx->system[r].constant;
        
        for (int f = 0; f < ctx->free_count; f++) {
            int f_idx = ctx->free_indices[f];
            val -= ctx->system[r].coeffs[f_idx] * solution[f_idx];
        }
        
        // Validation: Must be non-negative integer
        if (val < -0.001) return; 
        long rounded = (long)(val + 0.5);
        if (fabs(val - rounded) > 0.001) return;
        
        solution[p_idx] = (double)rounded;
        current_sum += rounded;
    }
    
    // 3. Check Consistency for any zero-rows (rows below pivot_row_count)
    for (int r = ctx->pivot_row_count; r < 10; r++) {
        double check = ctx->system[r].constant;
        for (int f = 0; f < ctx->free_count; f++) {
            int f_idx = ctx->free_indices[f];
            check -= ctx->system[r].coeffs[f_idx] * solution[f_idx];
        }
        if (fabs(check) > 0.001) return; // Inconsistent
    }

    // 4. Update Global Minimum
    if (current_sum < global_min_clicks) {
        global_min_clicks = current_sum;
    }
}

// Recursive helper to iterate free variables (standard C function)
void iterate_free_vars(SolverContext *ctx, int depth) {
    if (depth == ctx->free_count) {
        check_configuration(ctx);
        return;
    }

    // Iterate this free variable from 0 to search_limit
    for (int v = 0; v <= ctx->search_limit; v++) {
        ctx->free_vals[depth] = v;
        iterate_free_vars(ctx, depth + 1);
    }
}

void solve_fast(int n_buttons, short **buttons, short *goal) {
    Equation system[10];
    int pivot_col[10]; 
    
    // Dynamic upper bound for search
    long max_goal_val = 0;
    for (int r=0; r<10; r++) {
        if (goal[r] > max_goal_val) max_goal_val = goal[r];
    }
    // Safety buffer
    int search_limit = (int)max_goal_val + 10; 

    // 1. Initialize System
    for (int r = 0; r < 10; r++) {
        system[r].constant = (double)goal[r];
        pivot_col[r] = -1;
        for (int c = 0; c < n_buttons; c++) {
            system[r].coeffs[c] = 0.0;
            for (int k = 0; k < 10; k++) {
                if (buttons[c][k] == -1) break;
                if (buttons[c][k] == r) {
                    system[r].coeffs[c] = 1.0;
                    break;
                }
            }
        }
    }

    // 2. Gaussian Elimination (RREF)
    int pivot_row = 0;
    int is_free_var[20];
    for(int i=0; i<n_buttons; i++) is_free_var[i] = 1;

    for (int col = 0; col < n_buttons && pivot_row < 10; col++) {
        int pivot = pivot_row;
        for (int i = pivot_row + 1; i < 10; i++) {
            if (fabs(system[i].coeffs[col]) > fabs(system[pivot].coeffs[col])) pivot = i;
        }

        if (fabs(system[pivot].coeffs[col]) < 1e-9) continue;

        if (pivot != pivot_row) {
            Equation temp = system[pivot_row];
            system[pivot_row] = system[pivot];
            system[pivot] = temp;
        }

        double div = system[pivot_row].coeffs[col];
        for (int k = col; k < n_buttons; k++) system[pivot_row].coeffs[k] /= div;
        system[pivot_row].constant /= div;

        for (int i = 0; i < 10; i++) {
            if (i != pivot_row) {
                double fac = system[i].coeffs[col];
                for (int k = col; k < n_buttons; k++) system[i].coeffs[k] -= fac * system[pivot_row].coeffs[k];
                system[i].constant -= fac * system[pivot_row].constant;
            }
        }
        
        pivot_col[pivot_row] = col;
        is_free_var[col] = 0;
        pivot_row++;
    }

    // 3. Prepare Search Context
    int free_indices[20];
    int free_count = 0;
    for (int i = 0; i < n_buttons; i++) {
        if (is_free_var[i]) free_indices[free_count++] = i;
    }

    int free_vals[20];
    SolverContext ctx;
    ctx.system = system;
    ctx.pivot_col = pivot_col;
    ctx.pivot_row_count = pivot_row;
    ctx.free_indices = free_indices;
    ctx.free_count = free_count;
    ctx.search_limit = search_limit;
    ctx.free_vals = free_vals;

    // 4. Execute Search
    iterate_free_vars(&ctx, 0);
}

long Day10(short **goal, int goal_size, short ***buttons) {
    long total_result = 0;

    for (int i = 0; i < goal_size; i++) {
        printf("Processing goal %d/%d... ", i + 1, goal_size);
        fflush(stdout);

        int n_buttons = 0;
        while (n_buttons < max_button_size && buttons[i][n_buttons][0] != -1) n_buttons++;

        global_min_clicks = LONG_MAX;

        solve_fast(n_buttons, buttons[i], goal[i]);

        if (global_min_clicks != LONG_MAX) {
            printf("Success! Clicks: %ld\n", global_min_clicks);
            total_result += global_min_clicks;
        } else {
            printf("No solution found.\n");
        }
    }
    return total_result;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) { perror("Error"); return 1; }

    char line[1024];
    int count = 0;
    int case_num = 0;

    short ***buttons = malloc(number_of_lines * sizeof(short **));
    for (int i = 0; i < number_of_lines; i++) {
        buttons[i] = malloc(max_button_size * sizeof(short *));
        for (int j = 0; j < max_button_size; j++) {
            buttons[i][j] = malloc(max_number_of_lights * sizeof(short));
            for (int k = 0; k < max_number_of_lights; k++) buttons[i][j][k] = -1;
        }
    }

    short **goal = malloc(number_of_lines * sizeof(short *));
    for (int i = 0; i < number_of_lines; i++) {
        goal[i] = malloc(max_number_of_lights * sizeof(short));
        for (int j = 0; j < max_number_of_lights; j++) goal[i][j] = 0;
    }

    while (fgets(line, 1024, file)) {
        case_num = 0;
        int button_index = 0;
        if (count >= number_of_lines) {
            number_of_lines *= 2; 
            buttons = realloc(buttons, number_of_lines * sizeof(short **));
            goal = realloc(goal, number_of_lines * sizeof(short *));
            for(int n=count; n<number_of_lines; n++) {
                buttons[n] = malloc(max_button_size * sizeof(short *));
                for(int j=0; j<max_button_size; j++) {
                    buttons[n][j] = malloc(max_number_of_lights * sizeof(short));
                    for(int k=0; k<max_number_of_lights; k++) buttons[n][j][k] = -1;
                }
                goal[n] = malloc(max_number_of_lights * sizeof(short));
                for(int k=0; k<max_number_of_lights; k++) goal[n][k] = 0;
            }
        }
        
        for (char *p = line; *p != '\0'; p++) {
            if (*p == ' ') continue;
            if (*p == ']') case_num++;
            else if (*p == '{') case_num++;
            else if (case_num == 1) {
                int light_index = 0;
                while (*p != ')' && *p != ' ') {
                    char *end;
                    int val = strtol(p, &end, 10);
                    if (end != p) { buttons[count][button_index][light_index++] = val; p = end - 1; }
                    p++;
                }
                button_index++;
            } else if (case_num == 2) {
                int goal_index = 0;
                while (*p != '}') {
                    char *end;
                    int val = strtol(p, &end, 10);
                    if (end != p) { goal[count][goal_index++] = val; p = end - 1; }
                    p++;
                }
                break;
            }
        }
        count++;
    }

    long result = Day10(goal, count, buttons);
    printf("Result: %ld\n", result);
    return 0;
}
