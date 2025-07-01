#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

void print_disk_map(const std::vector<int>& disk_map) {
    for (int number : disk_map) {
        std::cout << number << " ";
    }
    std::cout << std::endl;
}

void print_blocks(const std::vector<int>& block_map) {
    for (int number : block_map) {
        if (number == -1) {
            std::cout << ".";
        }
        else {
            std::cout << number % 10;
        }
    }
    std::cout << std::endl;
}

std::vector<int> create_block_map(const std::vector<int>& disk_map) {
    std::vector<int> block_map;

    for (size_t i = 0; i < disk_map.size(); i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < disk_map[i]; j++) {
                block_map.push_back(i / 2);
            }
        }
        else {
            for (int k = 0; k < disk_map[i]; k++) {
                block_map.push_back(-1);
            }
        }
    }
    // print_blocks(block_map);
    return block_map;
}

void move_blocks(std::vector<int>& block_map) {
    int left = 0;
    int right = block_map.size() - 1;

    while (left < right) {
        while (left < right && block_map[left] != -1) {
           left++;
        }

        while (left < right && block_map[right] == -1) {
            right--;
        }

        if (left < right) {
            block_map[left] = block_map[right];
            block_map[right] = -1;
        }
    }
    // print_blocks(block_map);
}

long calculate_checksum(const std::vector<int>& disk_map) {
    long checksum = 0;

    std::vector<int> block_map = create_block_map(disk_map);
    move_blocks(block_map);

    for (size_t i = 0; i < block_map.size(); i++) {
        if (block_map[i] == -1) {
            break;
        }
        checksum += i * block_map[i];
        std::cout << "i: " << i << " block_map[i]: " << block_map[i] << " checksum: " << checksum << std::endl;
    }

    return checksum;
}

std::vector<int> read_input(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::vector<int> numbers;
    for (char c : line) {
        numbers.push_back(c - '0');
    }

    return numbers;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<int> disk_map = read_input(argv[1]);
    // print_disk_map(disk_map);

    long result = calculate_checksum(disk_map);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
