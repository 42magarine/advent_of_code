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
    int prizeX, prizeY;
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
//   => (prizeX - aX * a) / bX = b
// aY * a + bY * b = prizeY

int check_solution(const claw_maschine& claw_maschine) {
    for (int a = 0; a <= 100; a++) {
        if ((claw_maschine.prizeX - claw_maschine.aX * a) % claw_maschine.bX == 0) {
            int b = (claw_maschine.prizeX - claw_maschine.aX * a) / claw_maschine.bX;

            if (b >= 0 && b <= 100 &&
               (claw_maschine.aY * a + claw_maschine.bY * b == claw_maschine.prizeY)) {
                return a * 3 + b * 1;
            }
        }
    }
    return 0;
}

int calculate_tokens(const std::vector<claw_maschine>& list) {
    int tokens = 0;

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

        list.push_back({aX, aY, bX, bY, prizeX, prizeY});
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

    int result = calculate_tokens(input);
    std::cout << YELLOW << "Result: " << result << RESET << std::endl;

    return 0;
}
