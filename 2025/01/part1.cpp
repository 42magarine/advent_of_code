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
        int delta = (r.dir == 'L') ? -r.value : r.value;

        dial = (((dial + delta) % 100) + 100) % 100;

        if (dial == 0) {
            ++count;
        }

        std::cout << "The dial is rotated " << r.dir << r.value
             << " to point at " << dial << "." << std::endl;
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
