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

long test_match(const std::vector<std::string>& patterns, const std::string& design) {
    size_t n = design.size();
    std::vector<long> dp(n + 1, 0);
    dp[0] = 1; // dp[i] will store the number of ways to match the first i characters of design.

    for (size_t i = 1; i <= n; i++) {
        for (const auto& pattern : patterns) {
        // For each position i, iterate through all patterns in patterns.
        // Check if the current pattern can fit within the substring ending at position i:
        // The pattern length must be less than or equal to i.
        // The pattern must match the substring design[i - pat_len..i-1].

            size_t pat_len = pattern.size();
            if (i >= pat_len && design.compare(i - pat_len, pat_len, pattern) == 0) {
                dp[i] += dp[i - pat_len];
                // If the current pattern matches, update dp[i] by adding dp[i - pat_len]:
                // dp[i - pat_len] represents the number of ways to match the substring up to i - pat_len.
            }
        }
    }
    return dp[n];
    // After the loops, dp[n] contains the total number of ways to match the entire design string using the given patterns.
}

long test_match(const std::vector<std::string>& patterns, const std::string& design) {
    size_t n = design.size();
    std::vector<long> dp(n + 1, 0);
    dp[0] = 1;

    for (size_t i = 1; i <= n; i++) {
        for (const auto& pattern : patterns) {
            size_t pat_len = pattern.size();
            if (i >= pat_len && design.compare(i - pat_len, pat_len, pattern) == 0) {
                dp[i] += dp[i - pat_len];
            }
        }
    }
    return dp[n];
}

long count_matches(const std::vector<std::string>& patterns, const std::vector<std::string>& designs) {
    long result = 0;

    for (const auto& design : designs) {
        result += test_match(patterns, design);
        std::cout << design << " " << result << std::endl;
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

    long result = count_matches(patterns, designs);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
