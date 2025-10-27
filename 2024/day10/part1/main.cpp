#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

struct coor {
    size_t y;
    size_t x;

    bool operator<(const coor& other) const {
        return (y < other.y) || (y == other.y && x < other.x);
    }
};

void print_map(const std::vector<std::vector<int>>& map) {
    for (const auto& row : map) {
        for (int cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void check_trail(std::vector<std::vector<int>> map, int height, coor pos, std::set<coor>& reached_nines) {
    if (map[pos.y][pos.x] == 9) {
        reached_nines.insert(pos);
        return;
    }

    if (pos.y != 0 && map[pos.y - 1][pos.x] == height + 1) {
        check_trail(map, height + 1, {pos.y - 1, pos.x}, reached_nines);
    }
    if (pos.y + 1 < map.size() && map[pos.y + 1][pos.x] == height + 1) {
        check_trail(map, height + 1, {pos.y + 1, pos.x}, reached_nines);
    }
    if (pos.x != 0 && map[pos.y][pos.x - 1] == height + 1) {
        check_trail(map, height + 1, {pos.y, pos.x - 1}, reached_nines);
    }
    if (pos.x + 1 < map[pos.y].size() && map[pos.y][pos.x + 1] == height + 1) {
        check_trail(map, height + 1, {pos.y, pos.x + 1}, reached_nines);
    }
}

int calculate_score(std::vector<std::vector<int>>& map) {
    int score = 0;
    std::set<coor> reached_nines;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x] == 0) {
                check_trail(map, 0, {y, x}, reached_nines);
                score += reached_nines.size();
                reached_nines.clear();
            }
        }
    }
    return score;
}

std::vector<std::vector<int>> read_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<std::vector<int>> map;

    while (std::getline(file, line)) {
        std::vector<int> row;

        for (char c : line) {
            row.push_back(c - '0');
        }
        map.push_back(row);
    }

    file.close();
    return map;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> map = read_input(argv[1]);
    // print_map(map);

    int result = calculate_score(map);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
