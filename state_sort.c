#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"

int main() {
  int error_flag = 1;
  char *file_name = NULL;
  struct state *records = NULL;
  char c;
  int command = 0;

  read_file_name(&file_name, &error_flag);
  if (file_name == NULL) {
    error_flag = 0;
    printf("n/a\n");
    free(file_name);
    return 1;
  }

  FILE *file = fopen(file_name, "rb+");
  if (file == NULL) {
    error_flag = 0;
    printf("n/a\n");
    free(file_name);
    return 1;
  }

  int count = get_records_count_in_file(file);
  records = (struct state *)malloc(count * sizeof(struct state));
  if (records == NULL) {
    error_flag = 0;
    printf("n/a\n");
    free(file_name);
    fclose(file);
    return 1;
  }

  for (int i = 0; i < count; i++) {
    records[i] = read_record_from_file(file, i);
  }

  if (scanf("%d%c", &command, &c) != 2 || c != '\n') {
    error_flag = 0;
    printf("n/a\n");
  } else {
    switch (command) {
    case 0:
      print_file(file);
      break;
    case 1:
      for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
          if (compare(&records[j], &records[j + 1]) > 0) {
            swap_records_in_file(file, j, j + 1);
            struct state temp = records[j];
            records[j] = records[j + 1];
            records[j + 1] = temp;
          }
        }
      }
      print_file(file);
      break;
    case 2: {
      struct state new_record;
      read_new_record(&new_record, &error_flag);
      if (error_flag != 0) {
        write_record_in_file(file, records, 0);
      }
      print_file(file);
      break;
    }
    default:
      printf("n/a\n");
      break;
    }
  }

  free(file_name);
  free(records);
  fclose(file);

  return 0;
}

// считывание имени файла
void read_file_name(char **file_name, int *error_flag) {
  int n = 1;
  *file_name = (char *)malloc(n * sizeof(char));
  if (*file_name == NULL) {
    *error_flag = 0;
  }

  char ch;
  int i = 0;
  while (scanf("%c", &ch) == 1 && ch != '\n') {
    (*file_name)[i] = ch;
    i++;
    n++;
    *file_name = (char *)realloc(*file_name, n * sizeof(char));
    if (*file_name == NULL) {
      *error_flag = 0;
    }
  }
  (*file_name)[i] = '\0';
}

// считывание записи из файла по указанному индексу
struct state read_record_from_file(FILE *pfile, int index) {
  int offset = index * sizeof(struct state);
  fseek(pfile, offset, SEEK_SET);
  struct state record;
  fread(&record, sizeof(struct state), 1, pfile);
  rewind(pfile);
  return record;
}

//количество записей в файле
int get_records_count_in_file(FILE *pfile) {
  return get_file_size_in_bytes(pfile) / sizeof(struct state);
}

//размер файла в байтах
int get_file_size_in_bytes(FILE *pfile) {
  int size = 0;
  fseek(pfile, 0, SEEK_END);
  size = ftell(pfile);
  rewind(pfile);
  return size;
}

// замена местами двух записей в файле
void swap_records_in_file(FILE *pfile, int record_index1, int record_index2) {
  struct state record1 = read_record_from_file(pfile, record_index1);
  struct state record2 = read_record_from_file(pfile, record_index2);
  write_record_in_file(pfile, &record1, record_index2);
  write_record_in_file(pfile, &record2, record_index1);
}

// запись в файл по указанному индексу
void write_record_in_file(FILE *pfile, const struct state *record_to_write,
                          int index) {
  int offset = index * sizeof(struct state);
  fseek(pfile, offset, SEEK_SET);
  fwrite(record_to_write, sizeof(struct state), 1, pfile);
  fflush(pfile);
}

// сравнение двух записей для сортировки
int compare(const void *a, const void *b) {
  struct state *state_a = (struct state *)a;
  struct state *state_b = (struct state *)b;

  if (state_a->year != state_b->year)
    return state_a->year - state_b->year;
  if (state_a->month != state_b->month)
    return state_a->month - state_b->month;
  if (state_a->day != state_b->day)
    return state_a->day - state_b->day;
  if (state_a->hour != state_b->hour)
    return state_a->hour - state_b->hour;
  if (state_a->minute != state_b->minute)
    return state_a->minute - state_b->minute;
  if (state_a->sec != state_b->sec)
    return state_a->sec - state_b->sec;
  if (state_a->status != state_b->status)
    return state_a->status - state_b->status;
  return state_a->code - state_b->code;
}

void print_file(FILE *pfile) {
  int count = get_records_count_in_file(pfile);
  for (int i = 0; i < count; i++) {
    struct state record = read_record_from_file(pfile, i);
    printf("%d %d %d %d %d %d %d %d\n", record.year, record.month, record.day,
           record.hour, record.minute, record.sec, record.status, record.code);
  }
}

void read_new_record(struct state *new_record, int *error_flag) {
  if (scanf("%d %d %d %d %d %d %d %d", &new_record->year, &new_record->month,
            &new_record->day, &new_record->hour, &new_record->minute,
            &new_record->sec, &new_record->status, &new_record->code) != 8) {
    *error_flag = 0;
  }
}