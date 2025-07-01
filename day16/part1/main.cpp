#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

#define MOVE_COST 1
#define TURN_COST 1000

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
    int dir;
    int cost;

    bool operator==(const node& other) const {
        return pos == other.pos && dir == other.dir;
    }

    bool operator>(const node& other) const {
        return cost > other.cost;
    }
};

std::ostream& operator<<(std::ostream& os, const node& n) {
    os << "y: " << n.pos.y << " x: " << n.pos.x << " dir: " << n.dir << " cost: " << n.cost;
    return os;
}

void print_map(const std::vector<std::string>& map) {
    for (const auto& row : map) {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

coor find_position(const std::vector<std::string>& map, char position) {
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x] == position) {
                return {static_cast<int>(y), static_cast<int>(x)};
            }
        }
    }
    return {};
}

bool out_of_map(const std::vector<std::string>& map, const coor& pos) {
    return pos.y < 0 || pos.y >= static_cast<int>(map.size()) || pos.x < 0 || pos.x >= static_cast<int>(map[pos.y].size());
}

int find_lowest_score(const std::vector<std::string>& map) {
    const std::vector<coor> move = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // 0=up, 1=right, 2=down, 3=left
    std::priority_queue<node, std::vector<node>, std::greater<node>> pq;

    coor start = find_position(map, 'S');
    coor end = find_position(map, 'E');
    pq.push({start, 1, 0}); // The Reindeer start on the Start Tile (marked S) facing East

    std::vector<node> visited;

    while (!pq.empty()) {
        node current = pq.top();
        pq.pop();

        if (current.pos == end) {
            return current.cost;
        }

        if (std::find(visited.begin(), visited.end(), current) != visited.end()) {
            continue;
        }
        visited.push_back(current);

        for (int i = -1; i <= 1; i++) {
            node next;

            int new_dir = (current.dir + i + 4) % 4;
            next.pos = current.pos + move[new_dir];
            if (out_of_map(map, next.pos) || map[next.pos.y][next.pos.x] == '#') {
                continue;
            }

            next.dir = new_dir;
            if (current.dir == next.dir) {
                next.cost = current.cost + MOVE_COST;
            }
            else {
                next.cost = current.cost + TURN_COST + MOVE_COST;
            }
            pq.push(next);
        }
    }
    return 0;
}

std::vector<std::string> read_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file: '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<std::string> map;

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

    int result = find_lowest_score(map);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
