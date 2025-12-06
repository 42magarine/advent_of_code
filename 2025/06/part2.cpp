#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

/**
 * Reads the input file into a 2D character grid
 */
std::vector<std::vector<char>> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<std::vector<char>> grid;
    std::string line;

    while (std::getline(file, line)) {
        grid.emplace_back(line.begin(), line.end());
    }

    return grid;
}

/**
 * Solves math worksheet by reading columns right-to-left.
 * Each column forms a number (top-to-bottom), problems are separated by spaces
 */
long solveMathHomework(const std::vector<std::vector<char>>& grid) {
    long grandTotal = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<int> numbers;

    // Read columns from right to left
    for (int x = cols - 1; x >= 0; --x) {
        // Read the number from this column (top to bottom, excluding last row)
        int number = 0;

        for (int y = 0; y < rows - 1; ++y) {
            char c = grid[y][x];
            if (std::isdigit(c)) {
                number = number * 10 + (c - '0');
            }
        }
        numbers.push_back(number);

        // Check if this column contains an operator in the last row
        char op = grid[rows - 1][x];

        // Calculate result for current problem
        if (op == '+' || op == '*') {
            long result = (op == '+') ? 0 : 1;

            if (op == '+') {
                for (int n : numbers) {
                    result += n;
                }
            }
            else {  // op == '*'
                for (int n : numbers) {
                    result *= n;
                }
            }

            grandTotal += result;
            numbers.clear();
            --x;  // Skip the next column (it's always a space separator)
        }
    }

    return grandTotal;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    auto puzzleInput = parseInput(argv[1]);

    if (puzzleInput.empty()) {
        std::cerr << "Failed to parse input" << std::endl;
        return 1;
    }

    long result = solveMathHomework(puzzleInput);
    std::cout << "Grand total: " << result << std::endl;

    return 0;
}
