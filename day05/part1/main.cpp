#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

int find_number(int number, const std::vector<int>& update) {
    for (size_t i = 0; i < update.size(); i++) {
        if (number == update[i]) {
            return i;
        }
    }
    return -1;
}

int check_rules(const std::vector<std::pair<int, int>> rules, const std::vector<int> update) {
    for (const auto& rule : rules) {
        int first = find_number(rule.first, update);
        if (first == -1) {
            continue;
        }
        int second = find_number(rule.second, update);
        if (second == -1) {
            continue;
        }
        if (first > second) {
            return 0;
        }
    }

    return update[update.size() / 2];
}

int calculate_middle_number(const std::vector<std::pair<int, int>>& rules, const std::vector<std::vector<int>>& updates) {
    int result = 0;

    for (const auto& update : updates) {
        result += check_rules(rules, update);
    }

    return result;
}

void read_input(const std::string& file_path, std::vector<std::pair<int, int>>& rules, std::vector<std::vector<int>>& updates) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "': " << strerror(errno) << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::stringstream ss(line);
        int x;
        int y;
        char delimiter;

        ss >> x >> delimiter >> y;
        rules.emplace_back(x, y);
    }

    while (std::getline(file, line)) {
		std::stringstream ss(line);
        std::vector<int> numbers;
        int number;

        while (ss >> number) {
            numbers.push_back(number);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        updates.push_back(numbers);
    }

    file.close();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
    read_input(argv[1], rules, updates);

    int result = calculate_middle_number(rules, updates);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
