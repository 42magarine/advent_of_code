#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

void parseInput(const std::string& filename,
                std::vector<std::pair<long, long>>& ingredientRanges,
                std::vector<long>& ingredientIds) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    std::string line;

    // Read range lines
    while (std::getline(file, line)) {
        if (line.empty()) {
            break; // Empty line separates sections
        }

        std::stringstream ss(line);
        long begin, end;
        char delimiter;

        ss >> begin >> delimiter >> end;
        ingredientRanges.emplace_back(begin, end);
    }

    // Read ingredient IDs
    long id;
    while (file >> id) {
        ingredientIds.push_back(id);
    }
}

// Checks if ID is contained in at least one range
int countFreshIngredients(const std::vector<std::pair<long, long>>& ingredientRanges,
                          const std::vector<long>& ingredientIds) {
    int count = 0;

    for (long id : ingredientIds) {
        for (const auto& range : ingredientRanges) {
            if (id >= range.first && id <= range.second) {
                count++;
                break; // ID found in range, no need to check other ranges
            }
        }
    }

    return count;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::vector<std::pair<long, long>> ingredientRanges;
    std::vector<long> ingredientIds;

    parseInput(argv[1], ingredientRanges, ingredientIds);

    if (ingredientRanges.empty() || ingredientIds.empty()) {
        std::cerr << "Failed to parse input" << std::endl;
        return 1;
    }

    int result = countFreshIngredients(ingredientRanges, ingredientIds);
    std::cout << "Fresh ingredient IDs: " << result << std::endl;

    return 0;
}
