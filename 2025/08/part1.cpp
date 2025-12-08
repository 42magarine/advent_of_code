#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>

// Represents an edge (connection) between two points in 3D space
struct Edge {
    int pointA;           // Index of first point
    int pointB;           // Index of second point
    long long distance;   // Squared distance (sqrt omitted for performance)

    bool operator<(const Edge& other) const {
        return distance < other.distance;
    }
};

// Parse input file containing 3D coordinates (format: x,y,z per line)
std::vector<std::array<int, 3>> parseInput(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return {};
    }

    std::vector<std::array<int, 3>> points;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int x, y, z;
        char delimiter;

        if (ss >> x >> delimiter >> y >> delimiter >> z) {
            points.push_back({x, y, z});
        }
    }

    return points;
}

// Calculate squared Euclidean distance between two 3D points
// Returns squared distance (sqrt omitted since only needed for comparison)
long long distanceSquared(const std::array<int, 3>& a, const std::array<int, 3>& b) {
    long long dx = a[0] - b[0];
    long long dy = a[1] - b[1];
    long long dz = a[2] - b[2];
    return dx*dx + dy*dy + dz*dz;
}

// Calculate all pairwise distances and return edges sorted by distance
std::vector<Edge> calculateAllEdges(const std::vector<std::array<int, 3>>& points) {
    int size = points.size();
    std::vector<Edge> edges;

    // Calculate distance between every pair of points
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            edges.push_back({i, j, distanceSquared(points[i], points[j])});
        }
    }

    // Sort edges by distance (shortest first)
    std::sort(edges.begin(), edges.end());

    return edges;
}

// Connect points using the N shortest edges
// Returns cluster assignment for each point (points in same cluster have same ID)
std::vector<int> connectPoints(const std::vector<Edge>& edges, int numPoints, int numConnections) {
    // Initialize: each point starts in its own cluster (cluster ID = point index)
    std::vector<int> clusterID(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        clusterID[i] = i;
    }

    // Process the N shortest edges to merge clusters
    for (int i = 0; i < (int)edges.size() && i < numConnections; ++i) {
        int clusterA = clusterID[edges[i].pointA];
        int clusterB = clusterID[edges[i].pointB];

        // Merge clusters: move all points from clusterB to clusterA
        if (clusterA != clusterB) {
            for (int j = 0; j < numPoints; ++j) {
                if (clusterID[j] == clusterB) {
                    clusterID[j] = clusterA;
                }
            }
        }
    }

    return clusterID;
}

// Count how many points are in each cluster
std::vector<int> getClusterSizes(const std::vector<int>& clusterID) {
    // Count points per cluster
    std::unordered_map<int, int> clusterCounts;
    for (int id : clusterID) {
        ++clusterCounts[id];  // Default value is 0, then increment
    }

    // Extract sizes into vector
    std::vector<int> sizes;
    for (const auto& [clusterId, count] : clusterCounts) {
        sizes.push_back(count);
    }

    // Sort by size (largest first)
    std::sort(sizes.begin(), sizes.end(), std::greater<int>());

    return sizes;
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

    // Step 1: Calculate all edges and sort by distance
    auto edges = calculateAllEdges(points);

    // Step 2: Connect points using N shortest edges
    auto clusterID = connectPoints(edges, points.size(), 1000);

    // Step 3: Count cluster sizes
    auto clusterSizes = getClusterSizes(clusterID);

    // Step 4: Calculate product of 3 largest clusters
    int result = clusterSizes[0] * clusterSizes[1] * clusterSizes[2];

    std::cout << "Top 3 cluster sizes: " << clusterSizes[0] << " * "
                << clusterSizes[1] << " * " << clusterSizes[2]
                << " = " << result << std::endl;

    return 0;
}
