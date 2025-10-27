#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

constexpr int FALLEN_BYTES = 1024;
constexpr int MAP_SIZE = 71;

struct coor {
    int y;
    int x;

    bool operator==(const coor& other) const {
        return y == other.y && x == other.x;
    }

    coor operator+(const coor& other) const {
        return {y + other.y, x + other.x};
    }
};

struct node {
    coor pos;
    int steps;

    bool operator==(const node& other) const {
        return pos == other.pos;
    }

    bool operator>(const node& other) const {
        return steps > other.steps;
    }
};


void print_fallen_bytes(const std::vector<coor>& fallen_bytes) {
    for (const auto& coor : fallen_bytes) {
        std::cout << coor.x << "," << coor.y << std::endl;
    }
    std::cout << std::endl;
}

void print_map(const std::vector<std::string>& map) {
    for (const auto& row : map) {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

bool out_of_map(const coor& pos) {
    return pos.y < 0 || pos.y >= MAP_SIZE || pos.x < 0 || pos.x >= MAP_SIZE;
}

int find_path(std::vector<std::string>& map) {
    const std::vector<coor> move = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // 0=up, 1=right, 2=down, 3=left

    coor start = {0, 0};
    coor end = {MAP_SIZE - 1, MAP_SIZE - 1};

    std::priority_queue<node, std::vector<node>, std::greater<node>> queue;
    // std::queue<node> queue;
    queue.push({start, 0});

    std::vector<node> visited;

    while (!queue.empty()) {
        node current = queue.top();
        // node current = queue.front();
        queue.pop();
        std::cout << "y: " << current.pos.y << " x: " << current.pos.x << " steps: " << current.steps << std::endl;

        if (current.pos == end) {
            return current.steps;
        }

        if (std::find(visited.begin(), visited.end(), current) != visited.end()) {
            continue;
        }
        visited.push_back(current);

        for (const auto& dir : move) {
            node next;

            next.pos = current.pos + dir;
            if (out_of_map(next.pos) || map[next.pos.y][next.pos.x] == '#') {
                continue;
            }
            next.steps = current.steps + 1;
            queue.push(next);
        }
    }
    return 0;
}

std::vector<std::string> create_map(const std::vector<coor>& fallen_bytes) {
    std::vector<std::string> map(MAP_SIZE, std::string(MAP_SIZE, '.'));

    for (size_t i = 0; i < 1024 ; i++) {
        map[fallen_bytes[i].y][fallen_bytes[i].x] = '#';
    }

    // for (const auto& coor : fallen_bytes) {
    //     map[coor.y][coor.x] = '#';
    // }

    return map;
}

std::vector<coor> read_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file: '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<coor> fallen_bytes;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        coor temp;
        char comma;

        if (ss >> temp.x >> comma >> temp.y) {
            fallen_bytes.push_back(temp);
        }
    }
    file.close();
    return fallen_bytes;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<coor> fallen_bytes = read_input(argv[1]);
    print_fallen_bytes(fallen_bytes);

    std::vector<std::string> map = create_map(fallen_bytes);
    print_map(map);

    int result = find_path(map);
    print_map(map);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
