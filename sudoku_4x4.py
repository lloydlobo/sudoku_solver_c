from time import sleep


def println_at_wrap_end(i):
    if i % 4 != 0 and i != 0:
        print()


def print_sudoku(board) -> None:
    for i, row in enumerate(board):
        println_at_wrap_end(i)

        for cell in row:
            print(cell, end=" ")
    print()


def is_valid(board, row, col, num):
    # Check if 'num' is not already in the current row or column
    for i in range(4):
        if board[row][i] == num or board[i][col] == num:
            return False

    # Check if num is not in the 2x2 box.
    row_start, col_start = 2 * (row//2), 2 * (col//2)
    for i in range(2):
        for j in range(2):
            if board[row_start + i][col_start + j] == num:
                return False

    return True


def solve_sudoku(board):
    for row in range(4):
        for col in range(4):
            if board[row][col] == 0:
                sleep(0.2)
                print_sudoku(board)
                for num in range(1, 5):  # 1,2,3,4 are valid.
                    if is_valid(board, row, col, num):
                        board[row][col] = num
                        print(" "*8, "Trying to place", num, "at", row, col)
                        sleep(0.2)
                        # print_sudoku(board)
                        if solve_sudoku(board):
                            print(f"{' '*16} solved: {num} ({row}, {col})")
                            sleep(0.2)
                            return True
                        print(" "*16, "Backtracking from",
                              num, "at", row, col)
                        sleep(0.2)
                        board[row][col] = 0
                    else:
                        print(" "*8, "Invalid to place", num, "at", row, col)
                return False
            else:
                print("Has valid value",
                      board[row][col], "at", row, col)
                sleep(0.2)
                # print_sudoku(board)
    print("Outer True")
    return True


# Example Sudoku puzzle (0 represents empty cells)
sudoku_board = [
    [0, 0, 1, 0],
    [0, 0, 2, 0],
    [0, 0, 0, 0],
    [0, 0, 0, 0]
]

print("Initial Sudoku:")
print_sudoku(sudoku_board)
print()

if solve_sudoku(sudoku_board):
    print("Solved Sudoku:")
    print_sudoku(sudoku_board)
else:
    print("No solution found.")
