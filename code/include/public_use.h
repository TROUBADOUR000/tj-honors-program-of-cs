#pragma once

#define ROW 8
#define COL 10
#define MAX 1024

void print_special(const int length, const char p);

int menu(const char* p, const char* option);

void wait_for_key(const char* wait_key_value, const int X = -1, const int Y = -1);

void assign_arr(const int row, const int col, int arr[ROW][COL]);
void init_arr(const int row, const int col, int arr[ROW][COL], const int _max);
int max_arr(const int row, const int col, int arr[ROW][COL]);

void restore_mid(int mid[ROW][COL]);
int num_mid(const int row, const int col, int mid[ROW][COL]);

void swap(int* a, int* b);

int char_to_int(char* tmp);