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

        int x1 = points[i].first;
        int y1 = points[i].second;
        int x2 = points[j].first;
        int y2 = points[j].second;

        // Nur vertikale Kanten betrachten (horizontale überspringen)
        if (x1 == x2) {
            // Vertikale Kante
            int minY = std::min(y1, y2);
            int maxY = std::max(y1, y2);

            // Prüfe ob der Punkt auf der richtigen Höhe ist
            if (py >= minY && py < maxY) {
                // Liegt die Kante rechts vom Punkt?
                if (x1 > px) {
                    count++;
                }
            }

        }
    }

    // 1 (ungerade) = true / 0 (gerade) = false
    return count % 2 == 1;
}

bool isPointInPoints(const std::vector<std::pair<int,int>>& pts,
                     const std::pair<int,int>& p) {
    return std::find(pts.begin(), pts.end(), p) != pts.end();
}

long long findLargestRectangle(const std::vector<std::pair<int, int>>& points) {
    long long maxArea = 0;
    int numPoints = points.size();

    // Try all possible pairs of points as opposite corners
    for (int i = 0; i < numPoints; ++i) {
        for (int j = i + 1; j < numPoints; ++j) {
            int x1 = points[i].first;
            int y1 = points[i].second;
            int x2 = points[j].first;
            int y2 = points[j].second;

            std::pair<int, int> corner3 = {x1, y2};
            std::pair<int, int> corner4 = {x2, y1};

            bool c3Valid = isPointInPoints(points, corner3) ||
                           isPointInPolygon(points, corner3);

            bool c4Valid = isPointInPoints(points, corner4) ||
                           isPointInPolygon(points, corner4);

            if (!c3Valid || !c4Valid) {
                continue;
            }

            long long width = std::abs(x2 - x1) + 1;
            long long height = std::abs(y2 - y1) + 1;
            long long area = width * height;

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

    long long result = findLargestRectangle(points);
    std::cout << "Largest rectangle area: " << result << std::endl;

    return 0;
}
