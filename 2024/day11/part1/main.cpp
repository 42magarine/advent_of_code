#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

void print_stones(const std::vector<long>& stones) {
	for (long number : stones) {
		std::cout << number << " ";
	}
	std::cout << std::endl;
}

long split_first_half(long number) {
	std::string number_str = std::to_string(number);
	std::string first_half = number_str.substr(0, number_str.length() / 2);

	return std::stol(first_half);
}

long split_second_half(long number) {
	std::string number_str = std::to_string(number);
	std::string second_half = number_str.substr(number_str.length() / 2);

	return std::stol(second_half);
}

bool even_number_of_digits(long number) {
	std::string number_str = std::to_string(number);

	return number_str.length() % 2 == 0;
}

void blink(std::vector<long>& stones) {
	for (size_t i = 0; i < 25; i++) {
		for (size_t j = 0; j < stones.size(); j++) {
			if (stones[j] == 0) {
				stones[j] = 1;
			}
			else if (even_number_of_digits(stones[j])) {
				long first_half = split_first_half(stones[j]);
				long second_half = split_second_half(stones[j]);

				stones[j] = first_half;
				stones.insert(stones.begin() + j + 1, second_half);
				j++;
			}
			else {
				stones[j] *= 2024;
			}
		}
	}
}

std::vector<long> read_input(const std::string& file_path) {
	std::vector<long> stones;

	std::ifstream file(file_path);
	if (!file) {
		std::cerr << RED << "Error: Could not open file '" << file_path << "': " << strerror(errno) << RESET << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		long number;

		while (ss >> number) {
			stones.push_back(number);
		}
	}

	file.close();
	return stones;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
		return 1;
	}

	std::vector<long> stones = read_input(argv[1]);
	// print_stones(stones);

	blink(stones);
	int result = stones.size();
	std::cout << YELLOW << "Result: " << result << RESET << std::endl;

	return 0;
}
