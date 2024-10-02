#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct state {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    int status;
    int code;
};

void read_file_name(char **file_name, int *error_flag); 
struct state read_record_from_file(FILE *pfile, int index); 
int get_records_count_in_file(FILE *pfile); 
int get_file_size_in_bytes(FILE *pfile); 
void write_record_in_file(FILE *pfile, const struct state *record_to_write, int index); 
void swap_records_in_file(FILE *pfile, int record_index1, int record_index2); 
int compare(const void *a, const void *b); 
void print_file(FILE *pfile); 
void read_new_record(struct state *new_record, int *error_flag); 