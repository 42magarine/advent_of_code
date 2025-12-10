// Orthogonales Polygon:
//     Bei orthogonalen Polygonen treffen alle Seiten im rechten Winkel
//     aufeinander (das heißt, der Innenwinkel beträgt an jeder Seite
//     entweder 90° oder 270°).

// Punkt im Polygon
//     Es gibt einen einfachen Algorithmus, mit dem geprüft werden kann,
//     ob sich ein Punkt innerhalb eines Polygons in der Ebene befindet:
//     Es wird ein horizontaler Strahl durch den untersuchten Punkt gelegt
//     und untersucht, wie oft sich der Strahl mit den Kanten des Polygons
//     schneidet.
//     Der Punkt befindet sich innerhalb des Polygons, wenn die Anzahl der
//     Schnittpunkte rechts vom Punkt ungerade ist. Wenn die Anzahl gerade
//     ist, befindet sich der Punkt außerhalb.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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

bool isPointInPolygon(const std::vector<std::pair<int, int>>& points,
                      const std::pair<int, int>& p) {
    int count = 0;
    int px = p.first;
    int py = p.second;
    int n = points.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;  // The points wraps, so the first point is also connected to the last point.

        int x1 = points[i].first;  // x1 == x2
        // int x2 = points[j].first;
        int y1 = points[i].second;
        int y2 = points[j].second;

        // horizontale Kante skippen
        if (y1 == y2) {
            continue;
        }

        // check: liegt py zwischen y1 und y2?
        if (py < std::min(y1, y2) || py > std::max(y1, y2)) {
            continue;
        }

        // schneidet rechts vom Punkt?
        if (x1 > px) {
            count++;
        }
    }
    // std::cout << " count: " << count;
    return count % 2 == 1;  // 1 (ungerade) = true / 0 (gerade) = false
}

long findLargestRectangle(const std::vector<std::pair<int, int>>& points) {
    long maxArea = 0;
    int numPoints = points.size();

    // Try all possible pairs of points as opposite corners
    for (int i = 0; i < numPoints; ++i) {
        for (int j = i + 1; j < numPoints; ++j) {
            // corner1 = points[i] and corner2 = points[j]
            std::pair<int, int> corner3 = {points[i].first, points[j].second};
            std::pair<int, int> corner4 = {points[j].first, points[i].second};



            // pointC+D berechnen, wieviele Kanten ein Strahl nach rechts trifft
            // wenn gerade -> continue
            // wenn ungerade -> fläche berechnen und speichern
            if (!isPointInPolygon(points, corner3)) {
                // std::cout << " false\n";
                continue;
            }
            // std::cout << " true ";
            if (!isPointInPolygon(points, corner4)) {
                // std::cout << " false\n";
                continue;
            }
            // std::cout << " true\n";

            std::cout << "  A: (" << points[i].first << ", " << points[i].second << ")";
            std::cout << "  B: (" << points[j].first << ", " << points[j].second << ")";
            std::cout << "  C: (" << corner3.first << ", " << corner3.second << ")";
            std::cout << "  D: (" << corner4.first << ", " << corner4.second << ")\n";

            long width = std::abs(points[j].first - points[i].first) + 1;
            long height = std::abs(points[j].second - points[i].second) + 1;
            long area = width * height;
            maxArea = std::max(maxArea, area);
        }
    }

    return maxArea;
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

    long result = findLargestRectangle(points);
    std::cout << "Largest rectangle area: " << result << std::endl;

    return 0;
}
