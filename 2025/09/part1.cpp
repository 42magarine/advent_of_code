#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>

std::vector<std::pair<int, int>> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<std::pair<int, int>> points;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int x, y;
        char delimiter;
        if (ss >> x >> delimiter >> y) {
            points.push_back({x, y});
        }
    }

    return points;
}

long calculateLargestRectangle(const std::vector<std::pair<int, int>>& points) {
    // Find the maximum x and y coordinates to determine the bounding box
    int maxWidth = 0;
    int maxHeight = 0;

    for (const auto& [x, y] : points) {
        maxWidth = std::max(maxWidth, x);
        maxHeight = std::max(maxHeight, y);
    }

    // Find the four corner points that are furthest in each corner direction
    // Strategy: minimize the distance metric for each corner
    std::pair<int, int> topLeft, topRight, bottomLeft, bottomRight;
    int minTopLeftDist = std::numeric_limits<int>::max();
    int minTopRightDist = std::numeric_limits<int>::max();
    int minBottomLeftDist = std::numeric_limits<int>::max();
    int minBottomRightDist = std::numeric_limits<int>::max();

    for (const auto& [x, y] : points) {
        // Distance metric: sum of distances from respective corner
        int topLeftDist = x + y;
        int topRightDist = (maxWidth - x) + y;
        int bottomLeftDist = x + (maxHeight - y);
        int bottomRightDist = (maxWidth - x) + (maxHeight - y);

        if (topLeftDist < minTopLeftDist) {
            minTopLeftDist = topLeftDist;
            topLeft = {x, y};
        }
        if (topRightDist < minTopRightDist) {
            minTopRightDist = topRightDist;
            topRight = {x, y};
        }
        if (bottomLeftDist < minBottomLeftDist) {
            minBottomLeftDist = bottomLeftDist;
            bottomLeft = {x, y};
        }
        if (bottomRightDist < minBottomRightDist) {
            minBottomRightDist = bottomRightDist;
            bottomRight = {x, y};
        }
    }

    // Calculate two possible rectangles formed by different corner pairs
    // Rectangle 1: from top-left to bottom-right
    long width1 = static_cast<long>(bottomRight.first - topLeft.first) + 1;
    long height1 = static_cast<long>(bottomRight.second - topLeft.second) + 1;
    long area1 = width1 * height1;

    // Rectangle 2: from top-right to bottom-left
    long width2 = static_cast<long>(topRight.first - bottomLeft.first) + 1;
    long height2 = static_cast<long>(bottomLeft.second - topRight.second) + 1;
    long area2 = width2 * height2;

    // Return the larger area
    return std::max(area1, area2);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    auto points = parseInput(argv[1]);
    if (points.empty()) {
        std::cerr << "Error: Failed to parse input or file is empty" << std::endl;
        return 1;
    }

    long result = calculateLargestRectangle(points);
    std::cout << "Largest rectangle area: " << result << std::endl;

    return 0;
}
