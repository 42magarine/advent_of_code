#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/**
 * Parses the input file and returns a 2D grid of characters.
 * Each line in the file becomes a row in the grid.
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
 * Checks if a position in the grid contains a paper roll ('@').
 * Returns false if the position is out of bounds or empty.
 */
bool hasPaperAt(const std::vector<std::vector<char>>& grid, int row, int col) {
    size_t rows = grid.size();
    size_t cols = grid[0].size();

    // Check bounds - return false if out of bounds
    if (row < 0 || static_cast<size_t>(row) >= rows ||
        col < 0 || static_cast<size_t>(col) >= cols) {
        return false;
    }

    return grid[row][col] == '@';
}

/**
 * Counts how many of the 8 surrounding cells contain paper rolls ('@').
 */
int countAdjacentPaper(const std::vector<std::vector<char>>& grid, int row, int col) {
    int result = 0;

    // Offsets for 8 neighbouring directions
    static constexpr int offsets[8][2] = {
        { 0, 1}, // right
        { 0,-1}, // left
        { 1, 0}, // down
        {-1, 0}, // up
        { 1, 1}, // down-right
        { 1,-1}, // down-left
        {-1, 1}, // up-right
        {-1,-1}  // up-left
    };

    for (const auto& off : offsets) {
        result += hasPaperAt(grid, row + off[0], col + off[1]);
    }

    return result;
}

/**
 * Iterates over all cells marked with '@'
 * and counts how many have fewer than 4 neighbours.
 */
int countRemovablePaper(const std::vector<std::vector<char>>& grid) {
    int count = 0;
    size_t rows = grid.size();
    size_t cols = grid[0].size();

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            // Skip positions that aren't marked with '@'
            if (grid[row][col] != '@') {
                continue;
            }

            int neighbours = countAdjacentPaper(grid, row, col);
            if (neighbours < 4) {
                ++count;
            }
        }
    }

    return count;
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

    int result = countRemovablePaper(puzzleInput);
    std::cout << "Cells with fewer than 4 neighbours: " << result << std::endl;

    return 0;
}
