#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

void print_patterns(const std::vector<std::string>& patterns) {
    for (const auto& pattern : patterns) {
        std::cout << pattern << ", ";
    }
    std::cout << std::endl;
}

void print_desgins(const std::vector<std::string>& desgins) {
    for (const auto& desgin : desgins) {
        std::cout << desgin << std::endl;
    }
}

bool test_match(const std::vector<std::string>& patterns, const std::string& design) {
    if (design.empty()) {
        return true;
    }

    for (const auto& pattern : patterns) {
        if (design.compare(0, pattern.size(), pattern) == 0) {
            if (test_match(patterns, design.substr(pattern.size()))) {
                return true;
            }
        }
    }
    return false;
}

int count_matches(const std::vector<std::string>& patterns, const std::vector<std::string>& designs) {
    int result = 0;

    for (const auto& design : designs) {
        result += test_match(patterns, design);
    }

    return result;
}

void read_input(const std::string& file_path, std::vector<std::string>& patterns, std::vector<std::string>& designs) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file: '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && !patterns.empty()) {   // not first line nor empty lines
            designs.push_back(line);
        }
        else {  // first line
            std::stringstream ss(line);
            std::string pattern;
            while (std::getline(ss, pattern, ',')) {
            pattern.erase(0, pattern.find_first_not_of(" "));
            patterns.push_back(pattern);
            }
        }
    }

    file.close();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<std::string> patterns;
    std::vector<std::string> designs;
    read_input(argv[1], patterns, designs);
    // print_patterns(patterns);
    // print_desgins(designs);

    int result = count_matches(patterns, designs);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
