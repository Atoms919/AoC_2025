#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    unsigned long long sum = 0;

    while (fgets(line, sizeof line, file)) {
        char *ptr = line;
        short int tab[12]= { [0 ... 11] = 1 };
        while (*ptr) {
            if (isdigit((unsigned char)*ptr)) {
                char *end;
                short int c = *ptr - '0';
                for (int i=11; i>=0 ;i--){
                    if (tab[i]<c && isdigit((unsigned char)*(ptr+i))){
                        for(int j=0; j<i; j++){
                            tab[j]= 1;
                        }
                        tab[i]=c;
                        break;
                    }
                }
            }
            ptr++;
        }
        for (int i = 0; i < 12; i++) {
            sum += tab[i] * pow(10, i);
            //printf("%d", tab[11 - i]);
        }
        //printf("\n");
    }

    fclose(file);

    printf("Sum: %llu\n", sum);
    return 0;
}
