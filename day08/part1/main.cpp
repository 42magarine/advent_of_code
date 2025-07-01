#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

struct coor {
    int y;
    int x;

    coor(int y, int x) : y(y), x(x) {}

    coor operator+(const coor& other) const {
        return coor(y + other.y, x + other.x);
    }

    coor operator-(const coor& other) const {
        return coor(y - other.y, x - other.x);
    }

    bool in_bounds(int height, int width) const {
        return y >= 0 && y < height && x >= 0 && x < width;
    }
};

void print_map(const std::vector<std::string>& map) {
	for (const auto& row : map) {
        std::cout << row << std::endl;
    }
}

void print_antennas(std::unordered_map<char, std::vector<coor>>& antennas) {
    for (const auto& [key, positions] : antennas) {
        std::cout << "antenna: '" << key << "' found at positions: ";
        for (const auto& pos : positions) {
            std::cout << "[" << pos.y << ", " << pos.x << "] ";
        }
        std::cout << std::endl;
    }
}

std::unordered_map<char, std::vector<coor>> read_antennas(const std::vector<std::string>& map) {
    std::unordered_map<char, std::vector<coor>> antennas;

    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map[row].size(); col++) {
            char cell = map[row][col];
            if (cell != '.') {
                antennas[cell].emplace_back(row, col);
            }
        }
    }
    // print_antennas(antennas);
    return antennas;
}

int count_antinodes(std::vector<std::string>& map) {
    std::unordered_map<char, std::vector<coor>> antennas = read_antennas(map);

    int count = 0;
    for (const auto& antenna : antennas) {
        for (size_t i = 0; i < antenna.second.size(); i++) {
            for (size_t j = i + 1; j < antenna.second.size(); j++) {
                coor distance = antenna.second[j] - antenna.second[i];
                coor antinode_one = antenna.second[i] - distance;
                coor antinode_two = antenna.second[j] + distance;

                if (antinode_one.in_bounds(map.size(), map[0].size())) {
                    if (map[antinode_one.y][antinode_one.x] != '#') {
                        count++;
                    }
                    if (map[antinode_one.y][antinode_one.x] == '.') {
                        map[antinode_one.y][antinode_one.x] = '#';
                    }
                }

                if (antinode_two.in_bounds(map.size(), map[0].size())) {
                    if (map[antinode_two.y][antinode_two.x] != '#') {
                        count++;
                    }
                    if (map[antinode_two.y][antinode_two.x] == '.') {
                        map[antinode_two.y][antinode_two.x] = '#';
                    }
                }
            }
        }
    }
    return count;
}

std::vector<std::string> read_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::vector<std::string> map;
    std::string line;
    while (std::getline(file, line)) {
        map.push_back(line);
    }

    file.close();
    return map;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<std::string> map = read_input(argv[1]);
	// print_map(map);

	int result = count_antinodes(map);
    // print_map(map);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
