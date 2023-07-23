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

int main() {
    int status = 0;
    int work = 1;
    int user_inp;
    char* filename = NULL;
    FILE* file = NULL;

    /************MAIN MENU***************/
    while (work) {
        printf("\n");
        print_menu();
        printf(">> ");
        user_inp = get_number(&status);
        if (status) {
            printf("[ERR %d] can't get number\n", status);
            status = 0;
            continue;
        }

        if (user_inp == EXIT_CODE) {
            work = 0;
            continue;
        } else if (user_inp == RULES_LIST) {
            printf("FILES LIST\n\n");
            dirlist(RULES_DEFAULT_DIR);
        }// else if (user_inp == ) {
        //} else if (user_inp == ) {
    }
    /************MAIN MENU***************/

    return 0;
}

void print_menu() {
    printf("%d. List of rules\n", RULES_LIST);
    printf("%d. Exit\n", EXIT_CODE);
    //printf("\n");
}
