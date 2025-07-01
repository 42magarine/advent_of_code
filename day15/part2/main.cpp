#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

struct robot {
    size_t y, x;
};

void print_map(const std::vector<std::vector<char>>& map) {
    for (const auto& row : map) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void print_moves(const std::string& moves) {
    std::cout << moves << std::endl;
}

int sum_GPS(const std::vector<std::vector<char>>& map) {
    int result = 0;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x] == ']') {
                result += 100 * y + x - 1;
            }
        }
    }
    return result;
}

void move_boxes_up(std::vector<std::vector<char>>& map, int y, int x, char box) {
    if (map[y - 1][x] == '[') {
        move_boxes_up(map, y - 1, x, '[');
        move_boxes_up(map, y - 1, x + 1, ']');
    }
    else if (map[y - 1][x] == ']') {
        move_boxes_up(map, y - 1, x - 1, '[');
        move_boxes_up(map, y - 1, x, ']');
    }
    map[y - 1][x] = box;
    map[y][x] = '.';
}

void move_boxes_down(std::vector<std::vector<char>>& map, int y, int x, char box) {
    if (map[y + 1][x] == '[') {
        move_boxes_down(map, y + 1, x, '[');
        move_boxes_down(map, y + 1, x + 1, ']');
    }
    else if (map[y + 1][x] == ']') {
        move_boxes_down(map, y + 1, x - 1, '[');
        move_boxes_down(map, y + 1, x, ']');
    }
    map[y + 1][x] = box;
    map[y][x] = '.';
}

int check_walls_up(std::vector<std::vector<char>>& map, int y, int x) {
    int walls = 0;

    if (map[y - 1][x] == '#') {
        return 1;
    }
    else if (map[y - 1][x] == '.'){
        return 0;
    }
    else if (map[y - 1][x] == '[') {
        walls += check_walls_up(map, y - 1, x);
        walls += check_walls_up(map, y - 1, x + 1);
    }
    else if (map[y - 1][x] == ']') {
        walls += check_walls_up(map, y - 1, x - 1);
        walls += check_walls_up(map, y - 1, x);
    }
    return walls;
}

int check_walls_down(std::vector<std::vector<char>>& map, int y, int x) {
    int walls = 0;

    if (map[y + 1][x] == '#') {
        return 1;
    }
    else if (map[y + 1][x] == '.'){
        return 0;
    }
    else if (map[y + 1][x] == '[') {
        walls += check_walls_down(map, y + 1, x);
        walls += check_walls_down(map, y + 1, x + 1);
    }
    else if (map[y + 1][x] == ']') {
        walls += check_walls_down(map, y + 1, x - 1);
        walls += check_walls_down(map, y + 1, x);
    }
    return walls;
}

void move_bob(std::vector<std::vector<char>>& map, robot& bob, int y, int x) {
    map[bob.y][bob.x] = '.';
    bob.y += y;
    bob.x += x;
    map[bob.y][bob.x] = '@';
}

void move_up(std::vector<std::vector<char>>& map, robot& bob) {
    if (map[bob.y - 1][bob.x] == '#') {
        return;
    }
    if (map[bob.y - 1][bob.x] == '.') {
        move_bob(map, bob, -1, 0);
        return;
    }

    int walls = 0;
    if (map[bob.y - 1][bob.x] == '[') {
        walls += check_walls_up(map, bob.y - 1, bob.x);
        walls += check_walls_up(map, bob.y - 1, bob.x + 1);

        if (walls == 0) {
            move_boxes_up(map, bob.y - 1, bob.x, '[');
            move_boxes_up(map, bob.y - 1, bob.x + 1, ']');
            move_bob(map, bob, -1, 0);
        }
    }
    else if (map[bob.y - 1][bob.x] == ']') {
        walls += check_walls_up(map, bob.y - 1, bob.x - 1);
        walls += check_walls_up(map, bob.y - 1, bob.x);

        if (walls == 0) {
            move_boxes_up(map, bob.y - 1, bob.x - 1, '[');
            move_boxes_up(map, bob.y - 1, bob.x, ']');
            move_bob(map, bob, -1, 0);
        }
    }
}

void move_down(std::vector<std::vector<char>>& map, robot& bob) {
    if (map[bob.y + 1][bob.x] == '#') {
        return;
    }
    if (map[bob.y + 1][bob.x] == '.') {
        move_bob(map, bob, 1, 0);
        return;
    }

    int walls = 0;
    if (map[bob.y + 1][bob.x] == '[') {
        walls += check_walls_down(map, bob.y + 1, bob.x);
        walls += check_walls_down(map, bob.y + 1, bob.x + 1);

        if (walls == 0) {
            move_boxes_down(map, bob.y + 1, bob.x, '[');
            move_boxes_down(map, bob.y + 1, bob.x + 1, ']');
            move_bob(map, bob, 1, 0);
        }
    }
    else if (map[bob.y + 1][bob.x] == ']') {
        walls += check_walls_down(map, bob.y + 1, bob.x - 1);
        walls += check_walls_down(map, bob.y + 1, bob.x);

        if (walls == 0) {
            move_boxes_down(map, bob.y + 1, bob.x - 1, '[');
            move_boxes_down(map, bob.y + 1, bob.x, ']');
            move_bob(map, bob, 1, 0);
        }
    }
}

void move_right(std::vector<std::vector<char>>& map, robot& bob) {
    int count = 0;

    while (true) {
        if (map[bob.y][bob.x + 1 + count] == '#') {
            break;
        }
        if (map[bob.y][bob.x + 1 + count] == '[') {
            count += 2;
            continue;
        }
        if (map[bob.y][bob.x + 1 + count] == '.') {
            while (count != 0) {
                map[bob.y][bob.x + 1 + count] = ']';
                count--;
                map[bob.y][bob.x + 1 + count] = '[';
                count--;
                map[bob.y][bob.x + 1 + count] = '.';
            }
            move_bob(map, bob, 0, 1);
            break;
        }
    }
}

void move_left(std::vector<std::vector<char>>& map, robot& bob) {
    int count = 0;

    while (true) {
        if (map[bob.y][bob.x - 1 - count] == '#') {
            break;
        }
        if (map[bob.y][bob.x - 1 - count] == ']') {
            count += 2;
            continue;
        }
        if (map[bob.y][bob.x - 1 - count] == '.') {
            while (count != 0) {
                map[bob.y][bob.x - 1 - count] = '[';
                count--;
                map[bob.y][bob.x - 1 - count] = ']';
                count--;
                map[bob.y][bob.x - 1 - count] = '.';
            }
            move_bob(map, bob, 0, -1);
            break;
        }
    }
}

robot find_robot(const std::vector<std::vector<char>>& map) {
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x] == '@') {
                return robot {y, x};
            }
        }
    }
    return {};
}

void move_robot(std::vector<std::vector<char>>& map, const std::string& moves) {
    robot bob = find_robot(map);

    for (char c : moves) {
        if (c == '^') {
            move_up(map, bob);
        }
        else if (c == 'v') {
            move_down(map, bob);
        }
        else if (c == '<') {
            move_left(map, bob);
        }
        else if (c == '>') {
            move_right(map, bob);
        }
    }
}

std::vector<std::vector<char>> widen_map(const std::vector<std::vector<char>>& map) {
    std::vector<std::vector<char>> wide_map;

    for (const auto& row : map) {
        std::vector<char> new_row;

        for (char c : row) {
            if (c == '#') {
                new_row.push_back('#');
                new_row.push_back('#');
            }
            else if (c == 'O') {
                new_row.push_back('[');
                new_row.push_back(']');
            }
            else if (c == '.') {
                new_row.push_back('.');
                new_row.push_back('.');
            }
            else if (c == '@') {
                new_row.push_back('@');
                new_row.push_back('.');
            }
        }
        wide_map.push_back(new_row);
    }
    return wide_map;
}

void read_input(const std::string& file_path, std::vector<std::vector<char>>& map, std::string& moves) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file: '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        map.emplace_back(line.begin(), line.end());
    }

    while (std::getline(file, line)) {
        moves.append(line);
    }

    file.close();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> map;
    std::string moves;
    read_input(argv[1], map, moves);

    std::vector<std::vector<char>> wide_map = widen_map(map);
    move_robot(wide_map, moves);
    std::cout << YELLOW << "Result: " << sum_GPS(wide_map) << RESET << std::endl;

    return 0;
}
