#ifndef __IO_H__
#define __IO_H__

#define STR_LEN 256

/***********IN*********/
char* get_str(int* status);
char* get_sized_str(unsigned int size, int* status);
int get_number(int* status);
float get_float(int* status);
char* get_str();
/***********IN*********/

/***********OUT*********/
void print_raw_str(char* str);
int dirlist(char* dir_name);
/***********OUT*********/

/***********OTHER*********/
void flush_stdin();
/***********OTHER*********/

#endif
