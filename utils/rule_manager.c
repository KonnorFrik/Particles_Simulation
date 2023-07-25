#include <stdio.h>
#include <stdlib.h>
//#include <dirent.h>

#include "../io.h"
#include "../life/settings.h"
#include "menu.h"


/*
   Rule file struct
   int count
   int color
   int powers (count times)
 */
void print_menu();
void print_info();
void print_status(char* filename, char* dirname, FILE* file, int status);

int main() {
    int status = 0;
    int work = 1;
    int user_inp;
    char* dirname = RULES_DEFAULT_DIR;
    char* filename = NULL;
    FILE* file = NULL;
    print_info();

    /************MAIN MENU***************/
    while (work) {
        if (status) {
            printf("[ERR %d]\n", status);
            status = NEUTRAL;
        }

        printf("\n");
        printf("\n");
        print_status(filename, dirname, file, status);
        print_menu();

        printf(">> ");
        user_inp = get_number(&status);
        printf("\n======OUPUT======\n\n");

        if (status) {
            printf("[ERR %d] can't get number\n", status);
            if (!feof(stdin)) {
                flush_stdin();
            }
            user_inp = NEUTRAL;
            //continue;
        }

        if (user_inp == EXIT_CODE) {
            work = 0;
            continue;

        //} else if (user_inp == CLOSE_RULE) {
            //printf("CLOSE FILE\n");
            
        } else if (user_inp == OPEN_RULE) {
            printf("OPEN FILE\n");

        } else if (user_inp == OPEN_DIR) {
            if (!feof(stdin)) {
                flush_stdin();
            }
            printf("Dir name\n>> ");
            dirname = get_str(&status);
            if (dirname == NULL) {
                status = NULL_PTR;
            }
        
        } else {
            printf("Unknown command\n");
        }
        printf("\n======OUPUT======\n");
    }
    /************MAIN MENU***************/

    if (dirname != NULL) {
        free(dirname);
    }
    
    if (filename != NULL) {
        free(filename);
    }

    if (file != NULL) {
        fclose(file);
    }
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
    printf("%d. Open rule\n", OPEN_RULE);
    printf("%d. Open rules dir\n", OPEN_DIR);
    //new rule
    //del rule
    //printf("%d. List of rules\n", RULES_LIST);
    printf("%d. Exit\n", EXIT_CODE);
    //printf("\n");
}

void print_status(char* filename, char* dirname, FILE* file, int status) {
    //printf("\n");
    printf("--------Status------\n");
    printf("| Sys Status:   '%d'\n", status);
    printf("| Rule Dir:     '%s'\n", dirname == NULL ? "-NULL-" : dirname);
    printf("| Rule File:    '%s'\n", filename == NULL ? "-NULL-" : filename);
    printf("| Is Opened:     %s \n", file == NULL ? "NO" : "YES");
    printf("--------Status------\n");
    printf("\n");
}
