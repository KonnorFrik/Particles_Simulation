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
    //write_rule(filename, &status);
    //free(filename);
    //return status;
//}

void write_rule(char* filepath, int* status) {
    FILE* file_rule = fopen(filepath, "wb");

    if (file_rule == NULL) {
        if (DEBUG) {
            printf("Can't open rule: '%s'\n", filepath);
        }
        *status = NULL_PTR;
        return;
    }

    int count = 0;
    if (!*status) {
        printf("Count of teams\n>> ");
        count = get_number(status);
    }

    if (*status && DEBUG) {
        printf("Can't read count\n");
    }

    if (!*status) {
        if (fwrite(&count, sizeof(count), 1, file_rule) != 1) {
            printf("[ERR] can't write data\n");
            *status = NULL_PTR;
        }
    }

    int color = 0;
    float power;
    while (color < count && !*status) {
        if (fwrite(&color, sizeof(color), 1, file_rule) != 1) {
            *status = WRITE_ERR;
            continue;
        }

        if (DEBUG) {
            printf("Writed color: %d\n", color);
        }

        int powers_count = 0;
        while (powers_count < count && !*status) {
            printf("Power #%d for color: %d\n>> ", powers_count, color);
            power = get_float(status);
            if (*status) {
                if (DEBUG) {
                    printf("Can't read color\n");
                }
                continue;
            }

            if (fwrite(&power, sizeof(power), 1, file_rule) != 1) {
                *status = WRITE_ERR;
                continue;
            }

            powers_count++;
        }

        color++;
    }

    fclose(file_rule);
}
