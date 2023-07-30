#include <stdio.h>
#include <stdlib.h>
//#include <dirent.h>

#include "../io.h"
#include "../life/settings.h"
#include "manager.h"


/*
   Rule file struct
   int count
   int color
   int powers (count times)
 */
void print_menu();
void print_info();
void print_status(char* filename, int status);

int main() {
    int status = 0;
    int work = 1;
    int user_inp;

    //int changed_dir_flag = 0;
    //char* dirname = RULES_DEFAULT_DIR;

    char* filename = NULL;
    //FILE* file = NULL;
    print_info();

    /************MAIN MENU***************/
    while (work) {
        //if (status) {
            //printf("[ERR %d]\n", status);
            //status = NEUTRAL;
        //}

        printf("\n");
        printf("\n");
        print_status(filename, status);
        print_menu();

        if (status) {
            printf("[ERR %d] can't get number\n", status);
            if (!feof(stdin)) {
                if (DEBUG) { printf("[DEBUG] FLUSH STDIN\n"); }
                flush_stdin();
            }
            user_inp = NEUTRAL;
            status = NEUTRAL;
            continue;
        }

        printf(">> ");
        user_inp = get_number(&status);
        printf("\n======OUPUT======\n\n");


        if (user_inp == EXIT_CODE) {
            work = 0;
            continue;

            
        } else if (user_inp == SHOW_RULE) {
            if (!feof(stdin)) {
                flush_stdin();
            }

            printf("File path\n>> ");
            if (filename != NULL) {
                free(filename);
                filename = NULL;
            }
            filename = get_str(&status);

            if (filename != NULL) {
                read_rule(filename, &status);

            } else {
                printf("[ERR %d] Can't get filepath\n", status);
            }

        } else if (user_inp == NEW_RULE) {
            if (!feof(stdin)) {
                flush_stdin();
            }

            printf("File path\n>> ");
            if (filename != NULL) {
                free(filename);
                filename = NULL;
            }
            filename = get_str(&status);

            if (filename != NULL) {
                write_rule(filename, &status);

            } else {
                printf("[ERR %d] Can't get filepath\n", status);
            }

        } else if (user_inp == NEUTRAL) {
            continue;

        } else {
            printf("Unknown command\n");
        }
        printf("\n======OUPUT======\n");
    }
    /************MAIN MENU***************/

    //if (dirname != NULL && changed_dir_flag == 1) {
        //free(dirname);
    //}
    
    if (filename != NULL) {
        free(filename);
    }

    //if (file != NULL) {
        //fclose(file);
    //}

    printf("XXXXXXXXXXXXXXXXX\n");
    return 0;
}

FILE* choose_rule(char* dirname, int* status) {
    FILE* file = NULL;
    int count = dirlist(dirname);
    int answ = get_number(status);
    if (answ > count) {
        *status = INPUT_ERR;

    } else {
        printf("FAKE OPEN RULE\n");
    }

    return file;
}

FILE* open_rule(char* filename) {
    FILE* file = NULL;
    if (filename != NULL) {
        file = fopen(filename, FILEMODE);
    }
    return file;
}

FILE* close_rule(FILE* file) {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
    return file;
}

void print_info() {
    printf("\n");
    printf("=======================\n");
    printf("|     RULE MANAGER    |\n");
    printf("| For atom simulation |\n");
    printf("|  Author: KonnorFrik |\n");
    printf("=======================\n");
    printf("\n");
}

void print_menu() {
    //printf("%d. Close rule\n", CLOSE_RULE);
    printf("%d. Show rule\n", SHOW_RULE);
    printf("%d. New rule\n", NEW_RULE);
    printf("%d. Exit\n", EXIT_CODE);
    //printf("\n");
}

void print_status(char* filename, int status) {
    //printf("\n");
    printf("--------Status------\n");
    printf("| Sys Status:   '%d'\n", status);
    //printf("| Rule Dir:     '%s'\n", dirname == NULL ? "-NULL-" : dirname);
    printf("| Rule File:    '%s'\n", filename == NULL ? "-NULL-" : filename);
    //printf("| Is Opened:     %s \n", file == NULL ? "NO" : "YES");
    printf("--------Status------\n");
    printf("\n");
}
