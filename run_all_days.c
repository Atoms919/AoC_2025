#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main() {
    double total_time = 0;
    char cmd[512];
    const char* gcc = "C:/msys64/ucrt64/bin/gcc.exe";
    
    printf("=== AoC 2025 All Days Benchmark (Warmup + Stable) ===\n\n");

    for (int day = 1; day <= 11; day++) {
        char daystr[8]; 
        snprintf(daystr, sizeof(daystr), "Day%02d", day);
        printf("Day %d\n", day);
        
        {
            snprintf(cmd, sizeof(cmd), "%s -fdiagnostics-color=always -O3 -march=native -DNDEBUG %s/part_1.c -o %s_p1.exe", gcc, daystr, daystr);
            system(cmd); 
            
            snprintf(cmd, sizeof(cmd), "cd %s && ..\\%s_p1.exe >nul 2>&1 && cd ..", daystr, daystr);
            system(cmd);
            
            double min_time = 1e9;
            for(int run = 0; run < 3; run++) {
                snprintf(cmd, sizeof(cmd), "cd %s && ..\\%s_p1.exe >nul 2>&1 && cd ..", daystr, daystr);
                double start = get_time();
                system(cmd);
                double elapsed = get_time() - start;
                if(elapsed < min_time) min_time = elapsed;
            }
            total_time += min_time;
            printf("- Part 1: ");
            if (min_time > 2.0) {
                printf("\x1b[31m"); 
            } else if (min_time < 0.2) {
                printf("\x1b[32m"); 
            } else {
                printf("\x1b[33m");
            }
            printf("%.3f", min_time);
            printf("\x1b[0m"); 
            printf(" s\n");
        }
        
        {
            snprintf(cmd, sizeof(cmd), "%s -fdiagnostics-color=always -O3 -march=native -DNDEBUG %s/part_2.c -o %s_p2.exe", gcc, daystr, daystr);
            system(cmd);         
            
            snprintf(cmd, sizeof(cmd), "cd %s && ..\\%s_p2.exe >nul 2>&1 && cd ..", daystr, daystr); 
            system(cmd);    
            
            double min_time = 1e9;
            for(int run = 0; run < 3; run++) {
                snprintf(cmd, sizeof(cmd), "cd %s && ..\\%s_p2.exe >nul 2>&1 && cd ..", daystr, daystr);
                double start = get_time();
                system(cmd);
                double elapsed = get_time() - start;
                if(elapsed < min_time) min_time = elapsed;
            }
            total_time += min_time;

            printf("- Part 2: ");
            if (min_time > 2.0) {
                printf("\x1b[31m"); 
            } else if (min_time < 0.2) {
                printf("\x1b[32m"); 
            } else {
                printf("\x1b[33m");
            }
            printf("%.3f", min_time);
            printf("\x1b[0m");
            printf(" s\n");
        }
        printf("\n");
    }

    {
        snprintf(cmd, sizeof(cmd), "%s -fdiagnostics-color=always -O3 -march=native -DNDEBUG Day12/part_1.c -o day12_p1.exe", gcc);
        system(cmd); 
        
        snprintf(cmd, sizeof(cmd), "cd Day12 && ..\\day12_p1.exe >nul 2>&1 && cd ..");
        system(cmd); 
        
        double min_time = 1e9;
        for(int run = 0; run < 3; run++) {
            snprintf(cmd, sizeof(cmd), "cd Day12 && ..\\day12_p1.exe >nul 2>&1 && cd ..");
            double start = get_time();
            system(cmd);
            double elapsed = get_time() - start;
            if(elapsed < min_time) min_time = elapsed;
        }
        total_time += min_time;
        printf("Day 12\n");
        printf("- Part 1: ");
        if (min_time > 2.0) {
            printf("\x1b[31m"); 
        } else if (min_time < 0.2) {
            printf("\x1b[32m"); 
        } else {
            printf("\x1b[33m");
        }
        printf("%.3f", min_time);
        printf("\x1b[0m");
        printf(" s\n\n");
    }

    printf("\n=== Total Stable Runtime: %.3f s ===\n", total_time);
    system("del day*_p*.exe");
    return 0;
}
