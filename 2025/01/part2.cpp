#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Rotation {
    char dir;
    int value;
};

std::vector<Rotation> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<Rotation> input;
    std::string line;

    while (std::getline(file, line)) {
        Rotation r;
        r.dir = line[0];
        r.value = std::stoi(line.substr(1));
        input.push_back(r);
    }

    return input;
}

int computePassword(const std::vector<Rotation>& input) {
    int dial = 50;
    int count = 0;

    std::cout << "The dial starts by pointing at " << dial << "." << std::endl;

    for (const auto& r : input) {
        int pointAtZero = 0;

        if (r.dir == 'L') {
            // Calculate how many full -100 cycles we complete
            pointAtZero = std::abs((dial - r.value) / 100);

            // Check if we cross or land on 0 during the rotation
            // Don't count if we start at 0 (dial != 0)
            // Count if the result is 0 or negative (dial - r.value <= 0)
            if (dial != 0 && dial - r.value <= 0) {
                pointAtZero++;
            }

            // Normalize dial position to range 0-99
            // Double modulo ensures positive result even for negative numbers
            dial = (((dial - r.value) % 100) + 100) % 100;
        }
        else if (r.dir == 'R') {
            // Calculate how many full 100 cycles we complete
            pointAtZero = (dial + r.value) / 100;

            // Normalize dial position to range 0-99
            dial = (dial + r.value) % 100;
        }

        count += pointAtZero;

        std::cout << "The dial is rotated " << r.dir << r.value
             << " to point at " << dial
             << "; during this rotation, it points at 0 " << pointAtZero
             << "-times." << std::endl;
    }

    return count;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    auto puzzleInput = parseInput(argv[1]);
        if (puzzleInput.empty()) {
        std::cerr << "Failed to parse input" << std::endl;
        return 1;
    }

    int result = computePassword(puzzleInput);
    std::cout << "Password: " << result << std::endl;

    return 0;
}
