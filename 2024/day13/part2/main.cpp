#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define RESET  "\033[0m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"

struct claw_maschine {
    int aX, aY;
    int bX, bY;
    long long prizeX, prizeY;
};

void print_input(const std::vector<claw_maschine>& list) {
    for (const auto& claw_maschine : list) {
        std::cout << "Button A: X+" << claw_maschine.aX << ", Y+" << claw_maschine.aY << std::endl;
        std::cout << "Button B: X+" << claw_maschine.bX << ", Y+" << claw_maschine.bY << std::endl;
        std::cout << "Prize: X=" << claw_maschine.prizeX << ", Y=" << claw_maschine.prizeY << std::endl;
        std::cout << std::endl;
    }
}

// aX * a + bX * b = prizeX
//   => aX * a * aY + bX * b * aY = prizeX * aY
//   => aX * a * aY = prizeX * aY - bX * b * aY

// aY * a + bY * b = prizeY
//   => aY * a * aX + bY * b * aX = prizeY * aX
//   => aY * a * aX = prizeY * aX - bY * b * aX

// prizeX * aY - bX * b * aY = prizeY * aX - bY * b * aX
//   => bY * b * aX - bX * b * aY = prizeY * aX - prizeX * aY
//   => b (bY * aX - bX * aY) = prizeY * aX - prizeX * aY
//   => b = (prizeY * aX - prizeX * aY) / (bY * aX - bX * aY)

long long check_solution(const claw_maschine& claw_maschine) {
    long long denominator = claw_maschine.bY * claw_maschine.aX - claw_maschine.bX * claw_maschine.aY;
    if (denominator == 0) {
        return 0;
    }

    long long numerator = claw_maschine.prizeY * claw_maschine.aX - claw_maschine.prizeX * claw_maschine.aY;
    if (numerator % denominator != 0) {
        return 0;
    }

    long long b = numerator / denominator;
    if ((claw_maschine.prizeX - claw_maschine.bX * b) % claw_maschine.aX != 0) {
        return 0;
    }

    long long a = (claw_maschine.prizeX - claw_maschine.bX * b) / claw_maschine.aX;
    if (claw_maschine.aY * a + claw_maschine.bY * b == claw_maschine.prizeY) {
        return a * 3 + b * 1;
    }
    return 0;
}

long long calculate_tokens(const std::vector<claw_maschine>& list) {
    long long tokens = 0;

    for (const auto& claw_maschine : list) {
        tokens += check_solution(claw_maschine);
    }
    return tokens;
}

std::vector<claw_maschine> parse_input(const std::string& file_path) {
    std::vector<claw_maschine> list;

    std::ifstream file(file_path);
    if (!file) {
        std::cerr << RED << "Error: Could not open file: '" << file_path << "'" << RESET << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        int aX, aY, bX, bY, prizeX, prizeY;

        if (line.empty()){
            continue;
        }

        if (sscanf(line.c_str(), "Button A: X+%d, Y+%d", &aX, &aY) != 2) {
            std::cerr << RED << "Error: Invalid format in line: " << line << RESET << std::endl;
            exit(1);
        }

        if (!std::getline(file, line) || sscanf(line.c_str(), "Button B: X+%d, Y+%d", &bX, &bY) != 2) {
            std::cerr << RED << "Error: Invalid format in line: " << line << RESET << std::endl;
            exit(1);
        }

        if (!std::getline(file, line) || sscanf(line.c_str(), "Prize: X=%d, Y=%d", &prizeX, &prizeY) != 2) {
            std::cerr << RED << "Error: Invalid format in line: " << line << RESET << std::endl;
            exit(1);
        }

        list.push_back({aX, aY, bX, bY, prizeX + 10000000000000, prizeY + 10000000000000});
    }

    file.close();
    return list;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <input_file>" << RESET << std::endl;
        return 1;
    }

    std::vector<claw_maschine> input = parse_input(argv[1]);
    // print_input(input);

    long long result = calculate_tokens(input);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
