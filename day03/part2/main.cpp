#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <cstring>
#include <cerrno>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

#define RE R"(mul\(([0-9]{1,3}),([0-9]{1,3})\)|do\(\)|don't\(\))"

int calculate_mul(const std::vector<std::pair<int, int>>& mul) {
	int result = 0;

	for (const auto& [x, y] : mul) {
		result += x * y;
	}

	return result;
}

std::vector<std::pair<int, int>> read_input(const std::string& file_path) {
	std::vector<std::pair<int, int>> mul_matches;
	std::regex re(RE);

	std::ifstream file(file_path);
	if (!file) {
		std::cerr << RED << "Error: Could not open file '" << file_path << "': " << strerror(errno) << RESET << std::endl;
		exit(1);
	}

	std::string line;
	bool calculate = true;
	while (std::getline(file, line)) {
		auto begin = std::sregex_iterator(line.begin(), line.end(), re);
		auto end = std::sregex_iterator();

		for (auto it = begin; it != end; it++) {
			const std::smatch& match = *it;
			std::string match_str = match.str(0);

			if (match_str.find("mul") == 0 && calculate) {
				int x = std::stoi((*it)[1].str());
				int y = std::stoi((*it)[2].str());
				mul_matches.emplace_back(x, y);
			}
			else if (match_str == "do()") {
				calculate = true;
			}
			else if (match_str == "don't()") {
				calculate = false;
			}
		}
	}

	file.close();
	return mul_matches;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
		return 1;
	}

	std::vector<std::pair<int, int>> mul = read_input(argv[1]);

	int result = calculate_mul(mul);
	std::cout << YELLOW << "Result: " << result << RESET << std::endl;

	return 0;
}
