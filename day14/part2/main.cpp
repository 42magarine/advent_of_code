#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

#define ROWS 103
#define COLS 101

struct robot {
    int px, py;
    int vx, vy;
};

void print_input(const std::vector<robot>& robots) {
    for (const auto& robot : robots) {
        std::cout << "p=" << robot.px << "," << robot.py << " v=" << robot.vx << "," << robot.vy << std::endl;
    }
}

void print_map(const std::array<std::array<int, COLS>, ROWS>& map) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (map[i][j] == 0) {
                std::cout << ".";
            }
            else {
                std::cout << map[i][j];
            }
        }
        std::cout << std::endl;
    }
}

bool is_tree(const std::array<std::array<int, COLS>, ROWS>& map) {
    const std::vector<std::string> tree_pattern = {
        "1111111111111111111111111111111",
        "1.............................1",
        "1.............................1",
        "1.............................1",
        "1.............................1",
        "1..............1..............1",
        "1.............111.............1",
        "1............11111............1",
        "1...........1111111...........1",
        "1..........111111111..........1",
        "1............11111............1",
        "1...........1111111...........1",
        "1..........111111111..........1",
        "1.........11111111111.........1",
        "1........1111111111111........1",
        "1..........111111111..........1",
        "1.........11111111111.........1",
        "1........1111111111111........1",
        "1.......111111111111111.......1",
        "1......11111111111111111......1",
        "1........1111111111111........1",
        "1.......111111111111111.......1",
        "1......11111111111111111......1",
        "1.....1111111111111111111.....1",
        "1....111111111111111111111....1",
        "1.............111.............1",
        "1.............111.............1",
        "1.............111.............1",
        "1.............................1",
        "1.............................1",
        "1.............................1",
        "1.............................1",
        "1111111111111111111111111111111"
    };

    for (size_t i = 0; i < ROWS - tree_pattern.size(); i++) {
        for (size_t j = 0; j < COLS - tree_pattern[0].size(); j++) {
            bool matches = true;

            for (size_t row = 0; row < tree_pattern.size(); row++) {
                for (size_t col = 0; col < tree_pattern[row].size(); col++) {
                    if ((tree_pattern[row][col] == '1' && map[i + row][j + col] != 1) ||
                        (tree_pattern[row][col] == '.' && map[i + row][j + col] != 0)) {
                        matches = false;
                        break;
                    }
                }
                if (!matches) {
                    break;
                }
            }
            if (matches) {
                return true;
            }
        }
    }
    return false;
}

void place_robot(robot& robot, std::array<std::array<int, COLS>, ROWS>& map) {
    robot.py = (robot.py + robot.vy + ROWS) % ROWS;
    robot.px = (robot.px + robot.vx + COLS) % COLS;
    map[robot.py][robot.px] += 1;
}

int move_robots(std::vector<robot>& input) {
    int result = 0;
    std::array<std::array<int, COLS>, ROWS> map = {};

    while (true) {
        for (auto& robot : input) {
            place_robot(robot, map);
        }
        result++;

        if (is_tree(map)) {
            print_map(map);
            return result;
        }

        for (auto& row : map) {
            row.fill(0);
        }
    }
}

std::vector<robot> parse_input(const std::string& file_path) {
    std::vector<robot> robots;

    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file: '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        int px, py, vx, vy;

        if (sscanf(line.c_str(), "p=%d,%d v=%d,%d", &px, &py, &vx, &vy) != 4) {
            std::cerr << RED << "Error: Invalid format in line: " << line << RESET << std::endl;
            exit(1);
        }

        robots.push_back({px, py, vx, vy});
    }

    file.close();
    return robots;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<robot> input = parse_input(argv[1]);
    // print_input(input);

    int result = move_robots(input);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
