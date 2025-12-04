#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct IdRange {
    std::string firstId;
    std::string lastId;
};

// Parse input file containing comma-separated ranges like "11-22,95-115,998-1012"
std::vector<IdRange> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<IdRange> ranges;
    std::string line;

    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string range;

        while (std::getline(ss, range, ',')) {
            size_t dashPos = range.find('-');
            if (dashPos != std::string::npos) {
                ranges.push_back({
                    range.substr(0, dashPos),
                    range.substr(dashPos + 1)
                });
            }
        }
    }

    return ranges;
}

// Check if a string consists of a repeating pattern
// Uses mathematical trick:
// S is repeating if S appears in (S+S) before position |S|
bool isRepeated(const std::string& idAsString) {
    if (idAsString.size() < 2) {
        return false;
    }

    std::string doubled = idAsString + idAsString;

    // Search for the original string starting from position 1
    size_t pos = doubled.find(idAsString, 1);

    // If we find it before position size(), it means there's a repeating pattern
    return pos < idAsString.size();
}

// Compute sum of all invalid IDs across all ranges
long long computeSumOfInvalidIds(const std::vector<IdRange>& ranges) {
    long long sum = 0;

    for (const auto& range : ranges) {
        long long start = std::stoll(range.firstId);
        long long end = std::stoll(range.lastId);

        // Check each ID in the range
        for (long long i = start; i <= end; ++i) {
            if (isRepeated(std::to_string(i))) {
                std::cout << range.firstId << "-" << range.lastId
                          << " has invalid ID " << i << "." << std::endl;
                sum += i;
            }
        }
    }

    return sum;
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

    long long result = computeSumOfInvalidIds(puzzleInput);
    std::cout << "Sum of all invalid IDs: " << result << std::endl;

    return 0;
}
