#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
		return 1;
	}

	std::ifstream file(argv[1]);
	if (!file) {
		std::cerr << RED << "Error: Could not open file '" << argv[1] << "': " << strerror(errno) << RESET << std::endl;
		return 1;
	}

	std::vector<int> left_numbers;
	std::vector<int> right_numbers;
	int left;
	int right;

	while (file >> left >> right) {
		left_numbers.push_back(left);
		right_numbers.push_back(right);
	}

	file.close();

	int result = 0;

	while (!left_numbers.empty() && !right_numbers.empty()) {
		std::vector<int>::iterator min_left_it = std::min_element(left_numbers.begin(), left_numbers.end());
		std::vector<int>::iterator min_right_it = std::min_element(right_numbers.begin(), right_numbers.end());

		result += std::abs(*min_left_it - *min_right_it);

		left_numbers.erase(min_left_it);
		right_numbers.erase(min_right_it);
	}

	std::cout << YELLOW << "Result: " << result << RESET << std::endl;

	return 0;
}
