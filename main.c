#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef VERBOSE_FLAG  // gcc -o program program.c -DVERBOSE_FLAG
    #define DBG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DBG(fmt, ...)
#endif

FILE  *Infile;
size_t Sudoku_Grid[9][9];

const size_t ROW_SIZE  = sizeof(Sudoku_Grid) / sizeof(*Sudoku_Grid);  // 9.
const size_t COL_SIZE  = sizeof(Sudoku_Grid) / sizeof(*Sudoku_Grid);  // 9.
const size_t GRID_SIZE = sizeof(Sudoku_Grid) / sizeof(size_t);        // 81.

void usage(char *prog) {
    fprintf(stderr, "Incorrect usage:\nInput file not provided");
    fprintf(stderr, "Usage:\n\t%s <input-file>\n", prog);
}

void init_arrayc(char *array, size_t size) {
    memset(array, 0, sizeof(*array) * size);
}
void dbg_arrayc(char *array, size_t size, const char *fmt) {
    size_t counter = 0;
    for (size_t x = 0; x < size; x += 1) {
        DBG(fmt, array[x]);
        counter += 1;
    }
    DBG("\n");
    DBG("array_len: %zu\n", counter);
    DBG("\n");
}

void init_grid(size_t grid[ROW_SIZE][COL_SIZE]) {
    memset(grid, 0, sizeof(grid[0][0]) * GRID_SIZE);
}
void dbg_grid(size_t grid[ROW_SIZE][COL_SIZE]) {
    for (size_t x = 0; x < ROW_SIZE; x += 1) {
        for (size_t y = 0; y < COL_SIZE; y += 1) { DBG("%ld", grid[x][y]); }
        DBG("\n");
    }
    DBG("\n");
}
void draw_sudoku_grid(size_t grid[ROW_SIZE][COL_SIZE]) {
    for (size_t x = 0; x < ROW_SIZE; x += 1) {
        if (x % 3 == 0 && x != 0) { printf("-------------------------\n"); }
        for (size_t y = 0; y < COL_SIZE; y += 1) {
            if (y % 3 == 0 && y != 0) { printf(" | "); }
            printf("%2ld", grid[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

// Verifying that there are no duplicates in rows, columns, and 3x3 subgrids.
bool is_valid(size_t board[ROW_SIZE][COL_SIZE], size_t idx_row, size_t idx_col,
              size_t num) {
    for (size_t i = 0; i < ROW_SIZE; i += 1) {  // num is not in row or column.
        if (board[idx_row][i] == num || board[i][idx_col] == num) return false;
    }

    size_t row_start = 3 * (idx_row / 3), col_start = 3 * (idx_col / 3);
    for (size_t i = 0; i < 3; i += 1) {  // num is not in current subgrid.
        for (size_t j = 0; j < 3; j += 1) {
            if (board[row_start + i][col_start + j] == num) return false;
        }
    }

    return true;
}

// Implement simple backtracking.
bool solve_sudoku(size_t board[ROW_SIZE][COL_SIZE]) {
    for (size_t x = 0; x < ROW_SIZE; x += 1) {
        for (size_t y = 0; y < COL_SIZE; y += 1) {
            if (board[x][y] == 0) {
                for (size_t num = 1; num < 10; num += 1) {
                    if (is_valid(board, x, y, num)) {
                        board[x][y] = num;
                        if (solve_sudoku(board)) { return true; }
                        board[x][y] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }
    DBG("%s %s\n", argv[0], argv[1]);

    assert(ROW_SIZE == 9 && COL_SIZE == ROW_SIZE &&
           GRID_SIZE == (ROW_SIZE * COL_SIZE) && "Should be a 9X9 grid");
    init_grid(Sudoku_Grid);

    if ((Infile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "unable to open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    char         buf[GRID_SIZE];
    const size_t buf_cap  = sizeof(buf) / sizeof(char);
    size_t       buf_size = 0;

    assert(buf_cap == GRID_SIZE);
    init_arrayc(buf, buf_cap);

    // Populate 1D buffer.

    char c;
    while (((c = fgetc(Infile)) != EOF) && buf_size < GRID_SIZE) {
        if (c >= '0' && c <= '9') {
            buf[buf_size] = c - '0';  // Convert char to integer.
            buf_size += 1;
        }
        DBG("%c", c);
    }
    buf[GRID_SIZE] = '\0';
    DBG("\n\n");

    fclose(Infile);

    // Populate 2D Sudoku_Grid from 1D buffer.

    assert(buf_size == GRID_SIZE);

    size_t buf_index = 0;
    for (size_t row = 0; row < ROW_SIZE; row += 1) {
        for (size_t col = 0; col < COL_SIZE; col += 1) {
            Sudoku_Grid[row][col] = buf[buf_index];
            buf_index += 1;
        }
    }
    assert(buf_index == GRID_SIZE);
    dbg_grid(Sudoku_Grid);

    DBG("Draw initial sudoku: \n");
    draw_sudoku_grid(Sudoku_Grid);

    solve_sudoku(Sudoku_Grid);

    DBG("Draw solved sudoku:(todo!) \n");
    draw_sudoku_grid(Sudoku_Grid);

    return 0;
}
