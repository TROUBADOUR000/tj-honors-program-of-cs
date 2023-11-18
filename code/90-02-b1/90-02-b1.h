/* 2153592 ÐÅ01 ºúÒÝ·² */
#pragma once
void character_pattern(int choice);
void auto_solution();
void control(int choice);

int is_sudoku(int a[9][9], char fla[9][9]);
int is_solution(int a[9][9], char fla[9][9]);
void print(int a[9][9], char flag[9][9], char fla[9][9]);
int input(int a[9][9], char flag[9][9], int* steps, int* nx, char* ny, int* nu, char fla[9][9], int choice);

void draw(int a[9][9], char flag[9][9], char fla[9][9]);
void single(int n, char flag, int x, int y, char fla, int choice);
void back();
void temporary(int n, int x, int y);

void restore_fla(char fla[9][9]);
int check(int x, char y, int a[9][9], int n, char flag[9][9], char fla[9][9]);