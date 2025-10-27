#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

void print_lists(const std::vector<long>& results, const std::vector<std::vector<long>>& numbers) {
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << results[i] << ": ";
        for (long num : numbers[i]) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

long try_combinations(const std::vector<long>& numbers, size_t index, long current_result, long result) {
    if (index == numbers.size()) {
        return (current_result == result) ? result : 0;
    }

    long result_add = try_combinations(numbers, index + 1, current_result + numbers[index], result);
    if (result_add == result) {
        return result;
    }

    long result_mul = try_combinations(numbers, index + 1, current_result * numbers[index], result);
    if (result_mul == result) {
        return result;
    }

    return 0;
}

long calculate(const std::vector<long>& results, const std::vector<std::vector<long>>& numbers) {
    long result = 0;

    for (size_t i = 0; i < results.size(); i++) {
        result += try_combinations(numbers[i], 1, numbers[i][0], results[i]);
    }

    return result;
}

void read_input(const std::string& file_path, std::vector<long>& results, std::vector<std::vector<long>>& numbers) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        long result;
        char delimiter;
        ss >> result >> delimiter;
        results.push_back(result);

        std::vector<long> temp_numbers;
        long number;
        while (ss >> number) {
            temp_numbers.push_back(number);
        }
        numbers.push_back(temp_numbers);
    }

    file.close();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<long> results;
    std::vector<std::vector<long>> numbers;
    read_input(argv[1], results, numbers);
    print_lists(results, numbers);

    long result = calculate(results, numbers);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
