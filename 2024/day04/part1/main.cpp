#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

#define WORD "XMAS"

int find_word(const std::vector<std::vector<char>>& grid, int row, int col, int y, int x) {
    std::string word = WORD;
    int rows = grid.size();
    int cols = grid[0].size();

    for (size_t i = 0; i < word.size(); i++) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            return 0;
        }
        if (grid[row][col] != word[i]) {
            return 0;
        }
        row += y;
        col += x;
    }

    return 1;
}

int find_start(const std::vector<std::vector<char>>& grid) {
    int result = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            result += find_word(grid, row, col, 0, +1);   // right
            result += find_word(grid, row, col, 0, -1);   // left

            result += find_word(grid, row, col, +1, 0);   // down
            result += find_word(grid, row, col, -1, 0);   // up

            result += find_word(grid, row, col, +1, +1);  // down-right
            result += find_word(grid, row, col, +1, -1);  // down-left

            result += find_word(grid, row, col, -1, +1);  // up-right
            result += find_word(grid, row, col, -1, -1);  // up-left
        }
    }

    return result;
}

std::vector<std::vector<char>> read_input(const std::string& file_path) {
    std::vector<std::vector<char>> grid;

    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "': " << strerror(errno) << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }
    
    file.close();
    return grid;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> grid = read_input(argv[1]);

    int result = find_start(grid);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
