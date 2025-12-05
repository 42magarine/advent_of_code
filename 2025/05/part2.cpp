#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

std::vector<std::pair<long, long>> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return {};
    }

    std::string line;
    std::vector<std::pair<long, long>> ingredientRanges;

    // Read range lines
    while (std::getline(file, line)) {
        if (line.empty()) {
            break; // Empty line ends ranges section
        }

        std::stringstream ss(line);
        long begin, end;
        char delimiter;

        ss >> begin >> delimiter >> end;
        ingredientRanges.emplace_back(begin, end);
    }

    return ingredientRanges;
}

// Counts total unique IDs covered by all ranges (with overlap handling)
long countIngredientIds(std::vector<std::pair<long, long>>& ingredientRanges) {
    long count = 0;

    // Sort ranges by start position
    std::sort(ingredientRanges.begin(), ingredientRanges.end());

    long currentStart = ingredientRanges[0].first;
    long currentEnd = ingredientRanges[0].second;

    // Merge overlapping ranges and count
    for (size_t i = 1; i < ingredientRanges.size(); ++i) {
        long nextStart = ingredientRanges[i].first;
        long nextEnd = ingredientRanges[i].second;

        // Ranges overlap or are adjacent - merge them
        if (currentEnd + 1 >= nextStart) {
            currentEnd = std::max(currentEnd, nextEnd);
        }
        // No overlap - count current range and start new one
        else {
            count += (currentEnd - currentStart + 1);
            currentStart = nextStart;
            currentEnd = nextEnd;
        }
    }

    // Add the last range
    count += (currentEnd - currentStart + 1);

    return count;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    auto ingredientRanges = parseInput(argv[1]);
    if (ingredientRanges.empty()) {
        std::cerr << "Failed to parse input" << std::endl;
        return 1;
    }

    long result = countIngredientIds(ingredientRanges);
    std::cout << "Fresh ingredient IDs: " << result << std::endl;

    return 0;
}
