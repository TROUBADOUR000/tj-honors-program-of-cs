#pragma once
void array_element(int row, int col, int arr[10][10]);
void input_init(int* row, int* col, int* dst, char mid[10][10], int z);
int recursion_judge(int num, char a, int arr[10][10], char mid[10][10], int row, int col);
int is_end(int arr[10][10], int row, int col);

void restore_mid(char mid[8][10]);
int max_arr(int arr[8][10], int col, int row);
int num_mine(int row, int col, char mid[8][10]);
int is_end(int arr[8][10], int row, int col);

void swap(int* a, int* b);