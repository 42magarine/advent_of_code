#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

// #define ROWS 7
// #define COLS 11
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

int count_quadrant(const std::array<std::array<int, COLS>, ROWS>& map, int y_start, int y_end, int x_start, int x_end) {
    int result = 0;

    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            result += map[y][x];
        }
    }
    return result;
}

void place_robot(robot& robot, std::array<std::array<int, COLS>, ROWS>& map) {
    robot.py = (robot.py + (robot.vy * 100) % ROWS + ROWS) % ROWS;
    robot.px = (robot.px + (robot.vx * 100) % COLS + COLS) % COLS;

    map[robot.py][robot.px] += 1;
}

int move_robots(std::vector<robot>& input) {
    std::array<std::array<int, COLS>, ROWS> map = {};

    for (auto& robot : input) {
        place_robot(robot, map);
    }

    int up_left = count_quadrant(map, 0, (ROWS / 2), 0, (COLS / 2));
    int up_right = count_quadrant(map, (ROWS / 2) + 1, ROWS, 0, (COLS / 2));
    int down_left = count_quadrant(map, 0, (ROWS / 2), (COLS / 2) + 1, COLS);
    int down_right = count_quadrant(map, (ROWS / 2) + 1, ROWS, (COLS / 2) + 1, COLS);

    return up_left * up_right * down_left * down_right;
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
