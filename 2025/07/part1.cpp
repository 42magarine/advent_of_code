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
 * Recursively traces a beam split through the grid from position (y, x).
 */
int beam(std::vector<std::vector<char>>& grid, int y, int x) {
    // Check if position is out of bounds
    if (y >= (int)grid.size() || x < 0 || x >= (int)grid[0].size()) {
        return 0;
    }

    char current = grid[y][x];

    if (current == '^') {
        // Splitter found, count it
        int count = 1;

        // Beam splits to left and right
        count += beam(grid, y, x - 1);  // left beam
        count += beam(grid, y, x + 1);  // right beam

        return count;
    }
    else if (current == '.') {
        // Free space - mark as beam path
        grid[y][x] = '|';

        // Continue moving downward
        return beam(grid, y + 1, x);
    }

    return 0;
}

/**
 * Counts the total number of beam splitters encountered starting from 'S'.
 */
int countBeamSplit(std::vector<std::vector<char>>& grid) {
    // Find start position 'S' in first row
    int x = -1;
    for (size_t i = 0; i < grid[0].size(); ++i) {
        if (grid[0][i] == 'S') {
            x = i;
            break;
        }
    }

    // Start beam from position below S
    return beam(grid, 1, x);
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

    int result = countBeamSplit(puzzleInput);
    std::cout << "Beam split count: " << result << std::endl;

    // for (const auto& row : puzzleInput) {
    //     for (char c : row) {
    //         std::cout << c;
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
