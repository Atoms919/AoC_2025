#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    int sum = 0;

    while (fgets(line, sizeof line, file)) {
        char *ptr = line;
        short int units = 1;
        short int tens = 1;
        while (*ptr) {
            if (isdigit((unsigned char)*ptr)) {
                char *end;
                short int c = *ptr - '0';
                //printf("%d", c);
                if (c>tens && isdigit((unsigned char)*(ptr+1))){
                    tens = c;
                    units = 1;
                }
                else if (c>units){
                    units = c;
                }
                
            }
            ptr++;
        }
        sum += units + tens*10;
        //printf("%d\n", units + tens * 10);
    }

    fclose(file);

    printf("Sum: %d\n", sum);
    return 0;
}
