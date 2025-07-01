#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

struct plot {
    char plant_type;
    int area = 0;
    int perimeter = 0;
};

void print_map(const std::vector<std::string>& map) {
	for (const auto& row : map) {
		std::cout << row << std::endl;
	}
	std::cout << std::endl;
}

void flood_fill(std::vector<std::string>& map, int y, int x, plot& current_plot) {
    map[y][x] = current_plot.plant_type + 32; // Mark cell as visited
    current_plot.area++;

    // Direction vectors for neighbor cells (up, down, left, right)
    int dy[4] = {-1, 1, 0, 0};
    int dx[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int new_y = y + dy[i];
        int new_x = x + dx[i];

        // Check boundary or different plant type
        if (new_y < 0 || new_y >= static_cast<int>(map.size()) ||
            new_x < 0 || new_x >= static_cast<int>(map[y].size()) ||
            (map[new_y][new_x] != current_plot.plant_type && map[new_y][new_x] != current_plot.plant_type + 32)) {
            current_plot.perimeter++;
        }
        else if (map[new_y][new_x] == current_plot.plant_type) {
            flood_fill(map, new_y, new_x, current_plot);
        }
    }
}

int calculate_fencing_cost(std::vector<std::string>& map) {
    std::vector<plot> plots;

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[y].size(); x++) {
            if (map[y][x] >= 'A' && map[y][x] <= 'Z') {
                plot current_plot;
                current_plot.plant_type = map[y][x];

                flood_fill(map, y, x, current_plot);

                plots.push_back(current_plot);
            }
        }
    }

    int price = 0;

    for (const auto& plot : plots) {
        price += plot.area * plot.perimeter;
    }

    return price;
}

std::vector<std::string> read_input(const std::string& file_path) {
	std::ifstream file(file_path);
	if (!file) {
		std::cerr << RED << "Error: Could not open file '" << file_path << "'" << RESET << std::endl;
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
	print_map(map);

	int result = calculate_fencing_cost(map);
    print_map(map);
	std::cout << YELLOW << "Result: " << result << RESET << std::endl;

	return 0;
}
