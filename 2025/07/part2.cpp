#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

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
 * Recursively counts all paths from (y, x) to the bottom using memoization.
 */
long beam(const std::vector<std::vector<char>>& grid, int y, int x,
          std::vector<std::vector<long>>& memo) {
    // Check if position is out of bounds
    if (y >= (int)grid.size()) {
        return 1;  // Reached bottom - one valid path
    }
    if (x < 0 || x >= (int)grid[0].size()) {
        return 0;  // Out of bounds horizontally - invalid path
    }

    // Check if already calculated
    if (memo[y][x] != -1) {
        return memo[y][x];
    }

    char current = grid[y][x];
    long count = 0;

    if (current == '^') {
        // Splitter: sum paths from left and right branches
        count += beam(grid, y + 1, x - 1, memo);  // left beam
        count += beam(grid, y + 1, x + 1, memo);  // right beam
    }
    else if (current == '.') {
        // Free space: continue downward
        count += beam(grid, y + 1, x, memo);
    }

    // Store result in memo table
    memo[y][x] = count;
    return count;
}

/**
 * Counts all possible paths from 'S' to bottom using memoization.
 */
long countBeamPaths(std::vector<std::vector<char>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Initialize memoization table with -1 (not calculated)
    std::vector<std::vector<long>> memo(rows, std::vector<long>(cols, -1));

    // Find start position 'S' in first row
    int startX = -1;
    for (int x = 0; x < cols; ++x) {
        if (grid[0][x] == 'S') {
            startX = x;
            break;
        }
    }

    // Start beam from position below S
    return beam(grid, 1, startX, memo);
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

    long result = countBeamPaths(puzzleInput);
    std::cout << "Total beam paths: " << result << std::endl;

    return 0;
}
