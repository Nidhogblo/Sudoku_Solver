#include <iostream>

#define n 9
using namespace std;

class Sudoku {    
public:
    int matrix[n][n];

	Sudoku(int _matrix[9][9]);

    bool n_col(int col,int number);
    bool n_row(int row,int number);
    bool n_box(int row, int col,int number);

    bool empty(int& row, int& col);
    bool available(int row, int col, int num);
    
    bool solve();

};

Sudoku::Sudoku(int _matrix[9][9]) {
    for (int i = 0; i < 9; i++){
        for (int k = 0; k < 9; k++){
            matrix[i][k] =  _matrix[i][k];
        }    
    }   
    solve();
 }

bool Sudoku::n_col(int col,int number) {
    for (int row = 0; row < n; row++)
        if (matrix[row][col] == number)
            return true;
    return false;
}

bool Sudoku::n_row(int row, int number) {
    for (int col = 0; col < n; col++)
        if (matrix[row][col] == number)
            return true;
    return false;
}

bool Sudoku::n_box(int b_row, int b_col, int number) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (matrix[row + b_row][col + b_col] == number)
                return true;
    return false;
}

bool Sudoku::empty(int& row, int& col) { 
    for (row = 0; row < n; row++)
        for (col = 0; col < n; col++)
            if (matrix[row][col] == 0) 
                return true;
    return false;
}

bool Sudoku::available(int row, int col, int num) {
    
    return !n_row(row, num) && !n_col(col, num) && !n_box(row - row % 3,
        col - col % 3, num);
}

bool Sudoku::solve() {
    int row, col;
    if (!empty(row, col))
        return true;
    for (int num = 1; num <= 9; num++) { 
        if (available(row, col, num)) { 
            matrix[row][col] = num;
            if (solve()) 
                return true;
            matrix[row][col] = 0; 
        }
    }
    return false;
}