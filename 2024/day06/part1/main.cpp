#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

struct Coordinate {
    int y, x;

    Coordinate(int y = 0, int x = 0) : y(y), x(x) {}

    Coordinate operator+(const Coordinate& other) const {
        return Coordinate(y + other.y, x + other.x);
    }
};

void print_map(const std::vector<std::vector<char>>& grid) {
    for (const auto& row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

Coordinate find_guard(std::vector<std::vector<char>>& grid) {
    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == '^') {
                return Coordinate(y, x);
            }
        }
    }

    return Coordinate(-1, -1);
}

int walk_guard(std::vector<std::vector<char>>& grid) {
    Coordinate moves[4] = {
        Coordinate(-1, 0),  // up
        Coordinate(0, 1),   // right
        Coordinate(1, 0),   // down
        Coordinate(0, -1)   // left
    };

    Coordinate guard = find_guard(grid);
    if (guard.x == -1 && guard.y == -1) {
        std::cerr << RED << "Guard not found!" << RESET << std::endl;
        return -1;
    }

    int visited = 0;
    int direction = 0; // 0 = up, 1 = right, 2 = down, 3 = left

    while (true) {
        if (grid[guard.y][guard.x] != 'X') {
            grid[guard.y][guard.x] = 'X';
            visited++;
        }

        Coordinate next = guard + moves[direction];
        if (next.x < 0 || next.x >= static_cast<int>(grid[next.y].size()) ||
            next.y < 0 || next.y >= static_cast<int>(grid.size())) {
            return visited;
        }

        if (grid[next.y][next.x] == '#') {
            direction = (direction + 1) % 4;
        }
        else {
            guard = next;
        }
    }
}

std::vector<std::vector<char>> read_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "': " << strerror(errno) << RESET << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<std::vector<char>> grid;
    while (std::getline(file, line)) {
        grid.emplace_back(line.begin(), line.end());
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

    int result = walk_guard(grid);
    // print_map(grid);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
