/*
  Mizbah Ateeq
  COP 3223C - Lab Section 16
  A7: 2D Arrays
  April 5, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
  * board is a 2D integer array
  * everything needs to be default 0
  * bombs will b IDed by a -1
*/

void load_bombs(int rows, int columns, int board[rows][columns]) {

    srand(time(NULL));

    // number of bombs
    int bombs_num=30;

    // generate random num for rows and columns & place bomb @ random location
    for (int i=0; i<bombs_num; i++) {
        int rand_r=rand()%rows;
        int rand_c=rand()%columns;

        board[rand_r][rand_c] = -1;
    }
}

void load_hints(int rows, int columns, int board[rows][columns]) {

    for (int r=0; r<rows; r++) {
        for (int c=0; c<columns; c++) {
            // check to see if not bomb
            if (board[r][c] != -1) {
                int count = 0;
                for (int i=r-1; i<=r+1; i++) {
                    for (int j=c-1; j<=c+1; j++) {
                        if (i>=0 && i<rows && j>=0 && j<columns) {
                            if (board[i][j]==-1) {
                                count++;
                            }
                        }
                    }
                }
                board[r][c] = count;
            }
        }
    }
}

void print_board(int rows, int columns, int board[rows][columns]) {
    for (int r=0; r<rows; r++) {
        for (int c=0; c<columns; c++) {
            if (board[r][c] == -1) {
                printf("x");
            }
            else if (board[r][c]==0) {
                printf("-");
            }
            else {
                printf("%d", board[r][c]);
            }
        }
        printf("\n");
    }

    char file[100];
    printf("\nWhat is the name of the file you want to save the board to?\n");
    scanf("%s", file);

    //// Open the file for writing

    FILE *in= fopen(file, "w");

    for (int r=0; r<rows; r++) {
        for (int c=0; c<columns; c++) {
            if (board[r][c] == -1) {
                fprintf(in, "x");
            }
            else if (board[r][c]==0) {
                fprintf(in, "-");
            }
            else {
                fprintf(in, "%d", board[r][c]);
            }
        }
        fprintf(in, "\n");
    }
    fclose(in);
}
// loop row times
// loop column times
// if the value in the position is -1
//// fprint 'x'
// otherwise, if the value in the position is 0
//// fprint  '-'
// otherwise
//// fprint the number in the board
//// fprint a new line character

// Close file.


// Stage 1:

// (for) loop row times
// (for) loop column times
// if the value in the position is -1
// print 'x'
// otherwise if the value in the position is 0:
// print  '-'
// otherwise
// print the number in the board
// print a new line character

// Stage 2:

//// Ask the user for the file name and ave it in a char array

//// Open the file for writing

// loop row times
// loop column times
// if the value in the position is -1
//// fprint 'x'
// otherwise, if the value in the position is 0
//// fprint  '-'
// otherwise
//// fprint the number in the board
//// fprint a new line character

// Close file.

int main() {

    int width, height;

    // user input for width and height
    printf("What is the width of your board\n");
    scanf("%d", &width);
    printf("What is the height of your board?\n");
    scanf("%d", &height);

    // 2d array for board
    int board[height][width];

    // loop row times
    for (int r=0; r<height; r++) {
        // loop column times
        for (int c=0; c<width; c++) {
            board[r][c] = 0;
        }
    }

    load_bombs(height, width, board);
    load_hints(height, width, board);
    print_board(height, width, board);

    /*
    int r=5, c=6;
    int board [r][c];
    */

    // load_bombs(r,c, board);
    // load_bombs(r,c, board);

    return 0;
}
