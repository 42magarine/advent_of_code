#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

#define WORD "MAS"

int find_X(const std::vector<std::vector<char>>& grid, int row, int col) {
    int rows = grid.size();
    int cols = grid[0].size();

    if (row < 1 || row + 1 >= rows || col < 1 || col + 1 >= cols) {
        return 0;
    }

    if (((grid[row - 1][col - 1] == 'M' && grid[row + 1][col + 1] == 'S') || (grid[row - 1][col - 1] == 'S' && grid[row + 1][col + 1] == 'M')) &&
        ((grid[row - 1][col + 1] == 'M' && grid[row + 1][col - 1] == 'S') || (grid[row - 1][col + 1] == 'S' && grid[row + 1][col - 1] == 'M'))) {
        return 1;
    }

    return 0;
}

int find_start(const std::vector<std::vector<char>>& grid) {
    int result = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (grid[row][col] == 'A') {
                result += find_X(grid, row, col);
            }
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
