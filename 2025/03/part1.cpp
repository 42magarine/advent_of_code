#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Parse input file where each line contains single digits (0-9)
std::vector<std::vector<int>> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<std::vector<int>> grid;
    std::string line;

    while (std::getline(file, line)) {
        std::vector<int> row;

        for (char c : line) {
            // ASCII char to int conversion (implicit cast from char to int)
            row.push_back(c - '0');
        }
        grid.push_back(row);
    }

    return grid;
}

// Find position of the maximum value in the row (excluding the last position)
size_t posFirstBattery(const std::vector<int>& row) {
    int maxValue = 0;
    size_t maxPos = 0;

    for (size_t i = 0; i < row.size() - 1; ++i) {
        if (row[i] > maxValue) {
            maxValue = row[i];
            maxPos = i;
        }
    }

    return maxPos;
}

// Find position of the maximum value from start + 1 to the end of the row
size_t posSecondBattery(const std::vector<int>& row, size_t start) {
    int maxValue = 0;
    size_t maxPos = start + 1;

    for (size_t i = start + 1; i < row.size(); ++i) {
        if (row[i] > maxValue) {
            maxValue = row[i];
            maxPos = i;
        }
    }

    return maxPos;
}

// Calculate sum of voltages by combining first and second max digits
int sumOfVoltage(const std::vector<std::vector<int>>& grid) {
    const std::string boldOn = "\033[1m";
    const std::string boldOff = "\033[0m";

    int sum = 0;

    for (const auto& row : grid) {
        size_t firstPos = posFirstBattery(row);
        size_t secondPos = posSecondBattery(row, firstPos);

        // Print row with highlighted positions
        for (size_t i = 0; i < row.size(); ++i) {
            if (i == firstPos || i == secondPos) {
                std::cout << boldOn << row[i] << boldOff;
            }
            else {
                std::cout << row[i];
            }
        }
        std::cout << std::endl;

        // Combine first and second digit to form a two-digit number
        sum += (row[firstPos] * 10 + row[secondPos]);
    }

    return sum;
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

    int result = sumOfVoltage(puzzleInput);
    std::cout << "Sum of the maximum voltage: " << result << std::endl;

    return 0;
}
