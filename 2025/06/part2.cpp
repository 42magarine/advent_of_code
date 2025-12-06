#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
 * Checks if a character is a digit
 */
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

/**
 * Solves math worksheet by reading columns right-to-left.
 * Each column forms a number (top-to-bottom), problems are separated by spaces
 */
long solveMathHomework(const std::vector<std::vector<char>>& grid) {
    long grandTotal = 0;
    std::vector<int> numbers;
    int lastRow = grid.size() - 1;

    // Read columns from right to left
    for (int x = (int)grid[0].size() - 1; x >= 0; --x) {
        // Read the number from this column (top to bottom, excluding last row)
        int number = 0;

        for (size_t y = 0; y < grid.size() - 1; ++y) {
            if (isDigit(grid[y][x])) {
                number = number * 10 + (grid[y][x] - '0');
            }
        }
        numbers.push_back(number);

        char op = grid[lastRow][x];

        // Check if this column contains an operator in the last row
        if (op != '+' && op != '*') {
            continue;
        }
        // Calculate result for current problem
        else {
            long result = (op == '+') ? 0 : 1;

            if (op == '+') {
                for (int num : numbers) {
                    result += num;
                }
            }
            else if (op == '*') {
                for (int num : numbers) {
                    result *= num;
                }
            }

            numbers.clear();
            grandTotal += result;
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
