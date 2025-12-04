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

// Extract first half of a number string
// Examples: "11" -> "1", "998" -> "9", "1010" -> "10"
std::string getRangeStart(const std::string& num) {
    if (num.size() == 1) {
        return num;
    }

    return num.substr(0, num.size() / 2);
}

// Extract first half of a number string
// Examples: "11" -> "1", "998" -> "99", "1010" -> "10"
std::string getRangeEnd(const std::string& num) {
    if (num.size() == 1) {
        return num;
    }

    return num.substr(0, (num.size() + 1) / 2);
}

// Generate all potential invalid IDs within a range
// Strategy: Extract first halves of start/end, iterate through that range,
// then double each number to create invalid IDs (e.g., 10 -> "1010")
std::vector<std::string> findInvalidCandidates(const IdRange& range) {
    std::vector<std::string> candidates;

    // Get the first halves to determine the search space
    std::string startHalf = getRangeStart(range.firstId);
    std::string endHalf = getRangeEnd(range.lastId);

    // Convert to numbers for iteration
    long long startNum = std::stoll(startHalf);
    long long endNum = std::stoll(endHalf);

    // Generate all invalid IDs by doubling each number in range
    for (long long i = startNum; i <= endNum; ++i) {
        std::string half = std::to_string(i);
        candidates.push_back(half + half);
    }

    return candidates;
}

// Compute sum of all invalid IDs across all ranges
long long computeSumOfInvalidIds(const std::vector<IdRange>& ranges) {
    long long sum = 0;

    for (const auto& range : ranges) {
        long long start = std::stoll(range.firstId);
        long long end = std::stoll(range.lastId);

        std::vector<std::string> candidates = findInvalidCandidates(range);

        for (const std::string& candidate : candidates) {
            long long candidateNum = std::stoll(candidate);

            // Check if candidate is within the actual numeric range
            if (candidateNum >= start && candidateNum <= end) {
                std::cout << range.firstId << "-" << range.lastId
                          << " has invalid ID " << candidateNum << "."
                          << std::endl;
                sum += candidateNum;
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
