#include <stdio.h>
#include <stdlib.h>

#include "../io.h"
#include "../life/settings.h"
#include "manager.h"

/*
   Rule file struct
   int count
   int color
   int powers (count times)
 */


//int main() {
    //int status = 0;
    //printf("filename\n>> ");
    //char* filename = get_str(&status);
    //if (filename == NULL) {
        //printf("Can't get filename\n");
        //exit(status);
    //}
    //read_rule(filename, &status);
    //free(filename);
    //return status;
//}

void read_rule(char* filepath, int* status) {
    FILE* file_rule = fopen(filepath, "rb");

    if (file_rule == NULL) {
        *status = NULL_PTR;
        if (DEBUG) {
            printf("[ERR %d]Can't open rule: '%s'\n", *status, filepath);
        }
        //return;
    }

    int count = 0;
    //if (!*status) {
        //printf("Count of teams\n>> ");
        //count = get_number(status);
    //}

    if (*status && DEBUG) {
        printf("[ERR %d] Can't read count\n", *status);
    }

    if (!*status) {
        if (fread(&count, sizeof(count), 1, file_rule) == 1) {
            printf("\nCount of Teams (colors): %d\n", count);

        } else {
            printf("[ERR %d] can't read data\n", READ_ERR);
            *status = NULL_PTR;
        }
    }

    int color = 0;
    float power;
    while (color < (count - 1) && !*status) {
        if (fread(&color, sizeof(color), 1, file_rule) == 1) {
            printf("\nColor# %d\n", color);

        } else {
            *status = READ_ERR;
            if (DEBUG) {
                printf("[ERR %d] Can't read color\n", *status);
            }
            continue;
        }

        int powers_count = 0;
        while (powers_count < count && !*status) {

            if (fread(&power, sizeof(power), 1, file_rule) == 1) {
                //printf("\tPower #%d for color: %d\n>> ", powers_count, color);
                printf("\tPower# %d - %f\n", powers_count, power);

            } else {
                *status = READ_ERR;
                if (DEBUG) {
                    printf("[ERR %d] Can't read power\n", *status);
                }
                continue;
            }

            //if (*status) {
                //continue;
            //}

            powers_count++;
        }

        //color++;
    }

    if (file_rule != NULL) {
        fclose(file_rule);
    }
}

