#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/**
 * Parses the input file: reads number rows into grid, last line into operators
 */
void parseInput(const std::string& filename,
                std::vector<std::vector<int>>& numberGrid,
                std::vector<char>& operators) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string lastLine;

    // Read all lines
    while (std::getline(file, line)) {
        lastLine = line;

        // Try to parse as numbers
        std::istringstream ss(line);
        std::vector<int> row;
        int number;

        bool hasNumbers = false;
        while (ss >> number) {
            row.push_back(number);
            hasNumbers = true;
        }

        if (hasNumbers) {
            numberGrid.push_back(row);
        }
    }

    // Parse the last line as operators
    std::istringstream ss(lastLine);
    char op;
    while (ss >> op) {
        operators.push_back(op);
    }
}

/**
 * Solves math homework by calculating each column based on the operator
 * and returning the grand total.
 */
long solveMathHomework(const std::vector<std::vector<int>>& numberGrid,
                       const std::vector<char>& operators) {
    long grandTotal = 0;

    for (size_t i = 0; i < operators.size(); ++i) {
        long result = (operators[i] == '+') ? 0 : 1;

        if (operators[i] == '+') {
            for (const auto& row : numberGrid) {
                result += row[i];
            }
        }
        else if (operators[i] == '*') {
            for (const auto& row : numberGrid) {
                result *= row[i];
            }
        }

        grandTotal += result;
    }

    return grandTotal;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> numberGrid;
    std::vector<char> operators;

    parseInput(argv[1], numberGrid, operators);

    if (numberGrid.empty() || operators.empty()) {
        std::cerr << "Failed to parse input" << std::endl;
        return 1;
    }

    long result = solveMathHomework(numberGrid, operators);
    std::cout << "Grand total: " << result << std::endl;

    return 0;
}
