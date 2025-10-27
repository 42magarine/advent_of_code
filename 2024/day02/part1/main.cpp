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

bool is_valid_report(const std::vector<int>& report) {
	if (report.size() < 2) {
		return true;
	}

	bool is_increasing = report[0] < report[1];

	for (size_t i = 1; i < report.size(); i++) {
		int diff = is_increasing ? (report[i] - report[i - 1]) : (report[i - 1] - report[i]);
		if (diff < 1 || diff > 3) {
			return false;
		}
	}

	return true;
}

int count_valid_reports(const std::vector<std::vector<int>>& reports) {
	int valid_count = 0;

	for (const auto& report : reports) {
		if (is_valid_report(report)) {
			valid_count++;
		}
	}

	return valid_count;
}

std::vector<std::vector<int>> read_input(const std::string& file_path) {
	std::vector<std::vector<int>> reports;

	std::ifstream file(file_path);
	if (!file) {
		std::cerr << RED << "Error: Could not open file '" << file_path << "': " << strerror(errno) << RESET << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::vector<int> row;
		int number;

		while (ss >> number) {
			row.push_back(number);
		}

		reports.push_back(row);
	}

	file.close();
	return reports;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
		return 1;
	}

	std::vector<std::vector<int>> reports = read_input(argv[1]);

	int result = count_valid_reports(reports);
	std::cout << YELLOW << "Result: " << result << RESET << std::endl;

	return 0;
}
