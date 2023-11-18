#pragma once
void fundation(int z);
void full_edition();
void draw(int z);
void game();

void array_element(int row, int col, int arr[10][10]);
void input_init(int* row, int* col, int* dst, char mid[10][10], int z);
void input_tmp(int* num, char* a, int row, int col, int arr[10][10], char mid[10][10], int re);
void output_init(int row, int col, int arr[10][10]);
int judge(int num, char a, int arr[10][10], char mid[10][10], int row, int col);
int recursion_judge(int num, char a, int arr[10][10], char mid[10][10], int row, int col);
void output_ans(int row, int col, char mid[10][10]);
void output_now(int row, int col, char mid[10][10], int arr[10][10], int z);
char merge(int row, int col, char mid[10][10], int arr[10][10], int num, char a, int dst, int z, int m, int* gra);
void output_move(int row, int col, char mid[10][10], int arr[10][10], int dst, int z);
int by_mouse_keyboard(int x, int y, int z, int set_lines, int arr[10][10], int row, int col, char mid[10][10], int* colum, char* a);
void single_step(int x, int y, int z, int set_lines, int arr[10][10], int row, int col, char mid[10][10], int* column, char* a, int dst);
void whole_steps(int x, int y, int set_lines, int arr[10][10], int row, int col, char mid[10][10], int* column, char* a, int dst);
int is_end(int arr[10][10], int row, int col);

void print_single(int a, int z);
void print_background(int col, int set_lines, int set_cols, int c);
void clear();
void twinkle(int* column, char* a, int arr[8][10], char mid[8][10]);
void picture_5(int row, int col, int arr[10][10], int set_lines, int set_cols);
void picture_6(int row, int col, int arr[10][10], int set_lines, int set_cols);

void restore_mid(char mid[8][10]);
void restore_arr(int row, int col, int arr[8][10], char mid[8][10], int* column, char* a);
void restore_pic(int row, int col, int arr[8][10], char mid[8][10], int b_or_w);
int max_arr(int arr[8][10], int col, int row);
int num_mine(int row, int col, char mid[8][10]);
int is_end(int arr[8][10], int row, int col);
void random_fill(int row, int col, int arr[8][10], int z);
void fall(int row, int col, int arr[8][10], char mid[8][10], int z);

void swap(int* a, int* b);
void pre_grade(int sum, int* sum_grade, int t, int dst);
